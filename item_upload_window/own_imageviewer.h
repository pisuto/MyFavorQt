#ifndef OWN_IMAGEVIEWER_H
#define OWN_IMAGEVIEWER_H

#include <QWidget>

#include "own_config.h"

class QPixmap;

namespace mf {
    class OwnScreen;
}

namespace mf {

class OwnImageViewer : public QWidget
{
    Q_OBJECT
public:
    explicit OwnImageViewer(QLabel* display = Q_NULLPTR, QWidget* parent = Q_NULLPTR);
    ~OwnImageViewer() override {}

    void cropFinished();
    void setPixmap(QPixmap pix);
    const QPixmap& getFinalPixmap() const;
    void reset();
    void reset(const QString& path);

protected:
    void paintEvent(QPaintEvent* event) override ;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

private:
    void drawCrop(QPainter& painter);

private:
    QPixmap* mpPixmap;
    float mScale;
    OwnScreen* mpCropArea;
    bool mbStartMoving;
    QPoint mLastPos;
    QLabel* mpDisplay;
};

}

#endif // OWN_IMAGEVIEWER_H
