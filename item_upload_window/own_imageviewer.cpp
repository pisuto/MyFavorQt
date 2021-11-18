#include "own_imageviewer.h"
#include "own_screencapture.h"

#include <QPainter>
#include <QPaintEvent>

namespace mf {

OwnImageViewer::OwnImageViewer(QLabel* display, QWidget* parent) : QWidget (parent),
    mpPixmap(new QPixmap),
    mScale(1.f),
    mbStartMoving(false),
    mLastPos(QPoint(0, 0)),
    mpDisplay(display)
{
    mpPixmap->load(":/images/svgtopng/default.png");
    auto cropSize = OwnConfig::getInstance()->getDisplayImageSize();
    auto winSize = cropSize * 1.5;
    this->setFixedSize(winSize);
    mpCropArea = new OwnScreen(cropSize, winSize);
    // 加边框
    mpDisplay->setStyleSheet("border:1px solid black;");
}

void OwnImageViewer::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);
    if (mpPixmap->isNull())
        return;

    QPainter painter(this);
    painter.setRenderHint(QPainter::SmoothPixmapTransform);
    QSize pixSize = mpPixmap->size();

    //For canvas's size not change when window's size change.
    QSize initialSize = event->rect().size();
    // 使得长度保持一致
    mScale = 1.0f * initialSize.height() / pixSize.height();
    pixSize.scale(qreal(mScale) * pixSize, Qt::KeepAspectRatio);
    *mpPixmap = mpPixmap->scaled(pixSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    painter.drawPixmap(QPoint(0, 0), *mpPixmap);
    // 画切割线
    drawCrop(painter);
}

void OwnImageViewer::mousePressEvent(QMouseEvent *e)
{
    if ((e->buttons() == Qt::LeftButton))
    {
        if(mpCropArea->isInArea(e->pos()))
        {
            mbStartMoving = true;
            mLastPos = e->pos();
            this->setCursor(Qt::SizeAllCursor);
        }
    }
    this->update();
}

void OwnImageViewer::mouseMoveEvent(QMouseEvent *e)
{
    if ((e->buttons() == Qt::LeftButton) && mbStartMoving)
    {
        QPoint p(e->x() - mLastPos.x(), e->y() - mLastPos.y());
        mpCropArea->move(p);
        mLastPos = e->pos();
    }
    this->update();
}

void OwnImageViewer::mouseReleaseEvent(QMouseEvent *)
{
    mbStartMoving = false;
    this->setCursor(Qt::ArrowCursor);
}

void OwnImageViewer::drawCrop(QPainter &painter)
{
    painter.save();
    int x = mpCropArea->getLeftUp().x();
    int y = mpCropArea->getLeftUp().y();
    int w = mpCropArea->getWidth();
    int h = mpCropArea->getHeight();

    QPen pen;
    pen.setBrush(Qt::blue);
    pen.setStyle(Qt::DashLine);
    pen.setWidth(3);
    painter.setPen(pen);
    painter.drawRect(x, y, w, h);

    painter.restore();
}

void OwnImageViewer::cropFinished()
{

    QRect crop(mpCropArea->getLeftUp(), QSize(mpCropArea->getWidth(), mpCropArea->getHeight()));
    QPixmap cropped = mpPixmap->copy(crop);
    // 展示图片
    if(mpDisplay)
    {
        mpDisplay->setFixedSize(cropped.size());
        mpDisplay->setPixmap(cropped);
    }
    mbStartMoving = false;
    this->update();
}

void OwnImageViewer::setPixmap(QPixmap pix)
{
    if(!pix.isNull())
    {
        *mpPixmap = pix;
    }
}

}
