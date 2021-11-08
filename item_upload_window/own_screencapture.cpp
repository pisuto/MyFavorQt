#include "own_screencapture.h"

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

#define STRDATETIME qPrintable (QDateTime::currentDateTime().toString("yyyy-MM-dd-HH-mm-ss"))

void OwnScreen::move(QPoint p)
{
    int lx = leftUpPos.x() + p.x();
    int ly = leftUpPos.y() + p.y();
    int rx = rightDownPos.x() + p.x();
    int ry = rightDownPos.y() + p.y();

    if (lx < 0) {
        lx = 0;
        rx -= p.x();
    }

    if (ly < 0) {
        ly = 0;
        ry -= p.y();
    }

    if (rx > maxWidth)  {
        rx = maxWidth;
        lx -= p.x();
    }

    if (ry > maxHeight) {
        ry = maxHeight;
        ly -= p.y();
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
            QPoint tmp;
            tmp = leftTop;
            leftTop = rightDown;
            rightDown = tmp;
        }
    }
}

/*********************************************************************/

OwnScreenCapture::OwnScreenCapture(QWidget* parent) : QWidget(parent)
{
    menu = new QMenu(this);
    menu->addAction("Save screen", this, SLOT(saveScreen()));
    menu->addAction("Save full-screen", this, SLOT(saveFullScreen()));
    menu->addAction("Save screen as ...", this, SLOT(saveScreenOther()));
    menu->addAction("Save full-screen as ...", this, SLOT(saveFullOther()));
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
    painter.drawText(x + 2, y - 8, tr("截图范围：( %1 x %2 ) - ( %3 x %4 )  图片大小：( %5 x %6 )")
                     .arg(x).arg(y).arg(x + w).arg(y + h).arg(w).arg(h));
}

void OwnScreenCapture::showEvent(QShowEvent *)
{
    QPoint point(-1, -1);
    screen->setStart(point);
    screen->setEnd(point);

#if (QT_VERSION <= QT_VERSION_CHECK(5,0,0))
    *fullScreen = fullScreen->grabWindow(QApplication::desktop()->winId(), 0, 0, screen->width(), screen->height());
#else
    QScreen *pscreen = QApplication::primaryScreen();
    *fullScreen = pscreen->grabWindow(QApplication::desktop()->winId(), 0, 0, screen->width(), screen->height());
#endif

    //设置透明度实现模糊背景
    QPixmap pix(screen->width(), screen->height());
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

        QString fileName = QString("%1/screen_%2.png").arg(qApp->applicationDirPath()).arg(STRDATETIME);
        fullScreen->copy(x, y, w, h).save(fileName, "png");
        close();
}

void OwnScreenCapture::saveFullScreen()
{
    QString fileName = QString("%1/full_%2.png").arg(qApp->applicationDirPath()).arg(STRDATETIME);
        fullScreen->save(fileName, "png");
        close();
}

void OwnScreenCapture::saveScreenOther()
{
    QString name = QString("%1.png").arg(STRDATETIME);
        QString fileName = QFileDialog::getSaveFileName(this, "保存图片", name, "png Files (*.png)");
        if (!fileName.endsWith(".png")) {
            fileName += ".png";
        }

        if (fileName.length() > 0) {
            int x = screen->getLeftUp().x();
            int y = screen->getLeftUp().y();
            int w = screen->getRightDown().x() - x;
            int h = screen->getRightDown().y() - y;
            fullScreen->copy(x, y, w, h).save(fileName, "png");
            close();
        }
}

void OwnScreenCapture::saveFullOther()
{
    QString name = QString("%1.png").arg(STRDATETIME);
        QString fileName = QFileDialog::getSaveFileName(this, "保存图片", name, "png Files (*.png)");
        if (!fileName.endsWith(".png")) {
            fileName += ".png";
        }

        if (fileName.length() > 0) {
            fullScreen->save(fileName, "png");
            close();
        }
}


}
