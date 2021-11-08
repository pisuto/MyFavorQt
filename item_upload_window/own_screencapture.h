#ifndef OWN_SCREENCAPTURE_H
#define OWN_SCREENCAPTURE_H

#include <QWidget>

#include "own_config.h"

class QMenu;

namespace mf {

class OwnScreen
{
public:
    enum STATUS {SELECT, MOV, SET_W_H};
        OwnScreen() {}
        OwnScreen(QSize size) : maxWidth(size.width()),
            maxHeight(size.height()), startPos(QPoint(-1, -1)), endPos(startPos),
            leftUpPos(startPos), rightDownPos(startPos), status(SELECT) {}

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

        int width() const { return maxWidth; }
        int height() const { return maxHeight; }
        bool isInArea(QPoint pos) {         // 检测pos是否在截图区域内
            return pos.x() > leftUpPos.x() && pos.x() < rightDownPos.x() && pos.y() > leftUpPos.y() && pos.y() < rightDownPos.y();
        }
        void move(QPoint p);                // 按 p 移动截图区域

    private:
        QPoint leftUpPos, rightDownPos;     //记录 截图区域 左上角、右下角
        QPoint startPos, endPos;            //记录 鼠标开始位置、结束位置
        int maxWidth, maxHeight;            //记录屏幕大小
        STATUS status;                      //三个状态: 选择区域、移动区域、设置width height

        void cmpPoint(QPoint &s, QPoint &e);//比较两位置，判断左上角、右下角

};

class OwnScreenCapture : public QWidget
{
    Q_OBJECT
public:
    explicit OwnScreenCapture(QWidget* parent = Q_NULLPTR);

protected:
    void contextMenuEvent(QContextMenuEvent *);
    void mousePressEvent(QMouseEvent *);
    void mouseMoveEvent(QMouseEvent *);
    void mouseReleaseEvent(QMouseEvent *);
    void paintEvent(QPaintEvent *);
    void showEvent(QShowEvent *);

private slots:
    void saveScreen();
    void saveFullScreen();
    void saveScreenOther();
    void saveFullOther();

private:
    QMenu *menu;            //右键菜单对象
    OwnScreen *screen;      //截屏对象
    QPixmap *fullScreen;    //保存全屏图像
    QPixmap *bgScreen;      //模糊背景图
    QPoint movPos;          //坐标

};

}

#endif // OWN_SCREENCAPTURE_H
