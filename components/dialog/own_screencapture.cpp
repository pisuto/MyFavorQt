/**
* This file is part of MyFavor
*
* Copyright (C) 2021-2022 Pisuto.
*
*/

#include "own_screencapture.h"
#include "../global/own_config.h"

#include <QApplication>
#include <QPainter>
#include <QDesktopWidget>
#include <QFileDialog>
#include <QEvent>
#include <QDateTime>
#include <QStringList>
#include <QMouseEvent>
#include <QMenu>

namespace mf {

void OwnScreen::move(QPoint p)
{
    int lx = leftUpPos.x() + p.x();
    int ly = leftUpPos.y() + p.y();
    int rx = rightDownPos.x() + p.x();
    int ry = rightDownPos.y() + p.y();

    if (lx < 0) {
        rx = width;
        lx = 0;
    }

    if (ly < 0) {
        ry = height;
        ly = 0;
    }

    if (rx > maxWidth)  {
        lx = maxWidth - width;
        rx = maxWidth;
    }

    if (ry > maxHeight) {
        ly = maxHeight - height;
        ry = maxHeight;
    }
    leftUpPos = QPoint(lx, ly);
    rightDownPos = QPoint(rx, ry);
    startPos = leftUpPos;
    endPos = rightDownPos;
}

void OwnScreen::cmpPoint(QPoint &leftTop, QPoint &rightDown)
{
    QPoint l = leftTop;
    QPoint r = rightDown;

    if (l.x() <= r.x()) {
        if (l.y() <= r.y()) {
            ;
        } else {
            leftTop.setY(r.y());
            rightDown.setY(l.y());
        }
    } else {
        if (l.y() < r.y()) {
            leftTop.setX(r.x());
            rightDown.setX(l.x());
        } else {
            qSwap(leftTop, rightDown);
        }
    }
    // 设置方框的大小便于移动
    width  = qAbs(rightDown.x() - leftTop.x());
    height = qAbs(rightDown.y() - leftTop.y());
}

/*********************************************************************/

OwnScreenCapture::OwnScreenCapture(OwnImageViewer* viewer, QWidget* parent) :
    QWidget(parent),
    mpViewer(viewer)
{
    menu = new QMenu(this);
    menu->addAction("Save crop", this, SLOT(saveScreen()));
    menu->addAction("Exit", this, SLOT(hide()));

    //取得屏幕大小
    screen = new OwnScreen(QApplication::desktop()->size());
    //保存全屏图像
    fullScreen = new QPixmap();
}

void OwnScreenCapture::contextMenuEvent(QContextMenuEvent *)
{
    this->setCursor(Qt::ArrowCursor);
    menu->exec(cursor().pos());
}

void OwnScreenCapture::mousePressEvent(QMouseEvent *e)
{
    int status = screen->getStatus();

    if (status == OwnScreen::SELECT) {
        screen->setStart(e->pos());
    } else if (status == OwnScreen::MOV) {
        if (screen->isInArea(e->pos()) == false) {
            screen->setStart(e->pos());
            screen->setStatus(OwnScreen::SELECT);
        } else {
            movPos = e->pos();
            this->setCursor(Qt::SizeAllCursor);
        }
    }

    this->update();
}

void OwnScreenCapture::mouseMoveEvent(QMouseEvent *e)
{
    if (screen->getStatus() == OwnScreen::SELECT) {
        screen->setEnd(e->pos());
    } else if (screen->getStatus() == OwnScreen::MOV) {
        QPoint p(e->x() - movPos.x(), e->y() - movPos.y());
        screen->move(p);
        movPos = e->pos();
    }

    this->update();
}

void OwnScreenCapture::mouseReleaseEvent(QMouseEvent *)
{
    if (screen->getStatus() == OwnScreen::SELECT) {
        screen->setStatus(OwnScreen::MOV);
    } else if (screen->getStatus() == OwnScreen::MOV) {
        this->setCursor(Qt::ArrowCursor);
    }
}

void OwnScreenCapture::paintEvent(QPaintEvent *)
{
    int x = screen->getLeftUp().x();
    int y = screen->getLeftUp().y();
    int w = screen->getRightDown().x() - x;
    int h = screen->getRightDown().y() - y;

    QPainter painter(this);

    QPen pen;
    pen.setColor(Qt::green);
    pen.setWidth(2);
    pen.setStyle(Qt::DotLine);
    painter.setPen(pen);
    painter.drawPixmap(0, 0, *bgScreen);

    if (w != 0 && h != 0) {
        painter.drawPixmap(x, y, fullScreen->copy(x, y, w, h));
    }

    painter.drawRect(x, y, w, h);

    pen.setColor(Qt::yellow);
    painter.setPen(pen);
    painter.drawText(x + 2, y - 8, tr("Crop:( %1 x %2 ) - ( %3 x %4 )  Image:( %5 x %6 )")
                     .arg(x).arg(y).arg(x + w).arg(y + h).arg(w).arg(h));
}

void OwnScreenCapture::showEvent(QShowEvent *)
{
    QPoint point(-1, -1);
    screen->setStart(point);
    screen->setEnd(point);

#if (QT_VERSION <= QT_VERSION_CHECK(5,0,0))
    *fullScreen = fullScreen->grabWindow(QApplication::desktop()->winId(), 0, 0, screen->getMaxWidth(), screen->getMaxHeight());
#else
    QScreen *pscreen = QApplication::primaryScreen();
    *fullScreen = pscreen->grabWindow(QApplication::desktop()->winId(), 0, 0, screen->getMaxWidth(), screen->getMaxHeight());
#endif

    //设置透明度实现模糊背景
    QPixmap pix(screen->getMaxWidth(), screen->getMaxHeight());
    pix.fill((QColor(160, 160, 160, 200)));
    bgScreen = new QPixmap(*fullScreen);
    QPainter p(bgScreen);
    p.drawPixmap(0, 0, pix);
}

void OwnScreenCapture::saveScreen()
{
    int x = screen->getLeftUp().x();
    int y = screen->getLeftUp().y();
    int w = screen->getRightDown().x() - x;
    int h = screen->getRightDown().y() - y;
    mpViewer->setPixmap(fullScreen->copy(x, y, w, h));
    OwnConfig::getInstance()->handler(static_cast<MainWindow*>(Q_NULLPTR), HANDLER_OPER::OPER_SHOW);
    this->close();
}


}
