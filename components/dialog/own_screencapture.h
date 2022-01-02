#ifndef OWN_SCREENCAPTURE_H
#define OWN_SCREENCAPTURE_H

#include <QWidget>

#include "../global/own_config.h"
#include "own_imageviewer.h"

class QMenu;

namespace mf {

class OwnScreen
{
public:
    enum STATUS {SELECT, MOV, SET_W_H};
        OwnScreen() {}
        OwnScreen(QSize size) : maxWidth(size.width()),
            maxHeight(size.height()), startPos(QPoint(-1, -1)), endPos(startPos),
            leftUpPos(startPos), rightDownPos(startPos), width(0), height(0), status(SELECT) {}
        OwnScreen(QSize cropSize, QSize winSize) : leftUpPos(QPoint(0, 0)),
            rightDownPos(QPoint(cropSize.width(), cropSize.height())),
            startPos(QPoint(0, 0)), endPos(startPos), maxWidth(winSize.width()),
            maxHeight(winSize.height()), width(cropSize.width()),
            height(cropSize.height()) {}

        void setStart(QPoint pos) { startPos = pos; }
        void setEnd(QPoint pos) {
            endPos = pos;
            leftUpPos = startPos;
            rightDownPos = endPos;
            cmpPoint(leftUpPos, rightDownPos);
        }
        QPoint getStart() const { return startPos; }
        QPoint getEnd() const { return endPos; }

        QPoint getLeftUp() const { return leftUpPos; }
        QPoint getRightDown() const { return rightDownPos; }

        STATUS getStatus() const { return status; }
        void setStatus(STATUS status) { this->status = status; }

        int getWidth() const { return width; }
        int getHeight() const { return height; }
        int getMaxWidth() const { return maxWidth; }
        int getMaxHeight() const { return maxHeight; }
        bool isInArea(QPoint pos) {         // 检测pos是否在截图区域内
            return pos.x() > leftUpPos.x() && pos.x() < rightDownPos.x() && pos.y() > leftUpPos.y() && pos.y() < rightDownPos.y();
        }
        void move(QPoint p);                // 按 p 移动截图区域

        void reset(QSize cropSize = OwnConfig::getInstance()->getDisplayImageSize()) {
            this->setStart(QPoint(0, 0));
            this->setEnd(QPoint(0, 0));
            this->leftUpPos = QPoint(0, 0);
            this->rightDownPos = QPoint(cropSize.width(), cropSize.height());
        }

    private:
        QPoint leftUpPos, rightDownPos;         //记录 截图区域 左上角、右下角
        QPoint startPos, endPos;                //记录 鼠标开始位置、结束位置
        int maxWidth, maxHeight, width, height; //记录屏幕大小
        STATUS status;                          //三个状态: 选择区域、移动区域、设置width height

        void cmpPoint(QPoint &s, QPoint &e);//比较两位置，判断左上角、右下角

};

class OwnScreenCapture : public QWidget
{
    Q_OBJECT
public:
    explicit OwnScreenCapture(OwnImageViewer* viewer = Q_NULLPTR, QWidget* parent = Q_NULLPTR);

protected:
    void contextMenuEvent(QContextMenuEvent *);
    void mousePressEvent(QMouseEvent *);
    void mouseMoveEvent(QMouseEvent *);
    void mouseReleaseEvent(QMouseEvent *);
    void paintEvent(QPaintEvent *);
    void showEvent(QShowEvent *);

private slots:
    void saveScreen();

private:
    QMenu *menu;            //右键菜单对象
    OwnScreen *screen;      //截屏对象
    QPixmap *fullScreen;    //保存全屏图像
    QPixmap *bgScreen;      //模糊背景图
    QPoint movPos;          //坐标
    OwnImageViewer* mpViewer;
};

}

#endif // OWN_SCREENCAPTURE_H
