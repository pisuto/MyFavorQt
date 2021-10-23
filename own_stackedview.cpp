#include "own_stackedview.h"

#include <QPropertyAnimation>
#include <QPainter>
#include <QByteArray>

namespace mf {

OwnStackedView::OwnStackedView(QWidget* parent) : QStackedWidget (parent),
  mpAnime(new QPropertyAnimation(this, QByteArray())), mDuration(500), mbIsLeft(true),
  mbIsAnimation(false)
{
//    this->moveToThread(mpOwnThread);

    connect(mpAnime, &QPropertyAnimation::valueChanged, this, &OwnStackedView::valueChangedAnimation);
    connect(mpAnime, &QPropertyAnimation::finished, this, &OwnStackedView::animationFininshed);
}

void OwnStackedView::valueChangedAnimation(QVariant value)
{
    this->mCurrentVal = value;
    this->update();
}

void OwnStackedView::animationFininshed()
{
    mbIsAnimation = false;
    this->widget(this->currentIndex())->show();
    this->setCurrentIndex(mNextIdx);

//    mpOwnThread->quit();
//    mpOwnThread->wait();
}

void OwnStackedView::nextWidget()
{
    if(mbIsAnimation)
        return;
    mbIsLeft = true;
    mWidgetCnt = this->count();
    int index = this->currentIndex();
    mNextIdx = (index + 1) % mWidgetCnt;
    startAnimation(index);
}

void OwnStackedView::previousWidget()
{
    if(mbIsAnimation)
        return;
    mbIsLeft = false;
    mWidgetCnt = this->count();
    int index = this->currentIndex();
    mNextIdx = (index - 1 + mWidgetCnt) % mWidgetCnt;
    startAnimation(index);
}

void OwnStackedView::startAnimation(int index)
{
    mbIsAnimation = true;
    this->widget(index)->hide();
    QRect rect = this->geometry();
    mpAnime->setStartValue(rect.width());
    mpAnime->setEndValue(rect.x());
    mpAnime->setDuration(mDuration);
    mpAnime->setEasingCurve(QEasingCurve::InOutQuad);
    mpAnime->start();
}


void OwnStackedView::paintEvent(QPaintEvent *event)
{
    if(!mbIsAnimation)
        return;
    QPainter painter(this);
    paintNext(painter, mNextIdx);
    paintPrevious(painter, this->currentIndex());
}

void OwnStackedView::paintPrevious(QPainter &painter, int index)
{
    QWidget *widget = this->widget(index);
    QPixmap pixmap((widget->size()));
    widget->render(&pixmap);
    QRect rect = widget->geometry();

    double value = mCurrentVal.toDouble();
    QRectF r1(0.0, 0.0, value, rect.height());
    QRectF r2(rect.width() - value, 0, value, rect.height());
    if(mbIsLeft)
    {
        painter.drawPixmap(r1, pixmap, r2);
    }
    else
    {
        painter.drawPixmap(r2, pixmap, r1);
    }
}

void OwnStackedView::paintNext(QPainter &painter, int index)
{
    QWidget *nextWidget = this->widget(index);
    QRect rect = this->geometry();
    nextWidget->resize(rect.width(), rect.height());
    QPixmap nextPixmap(nextWidget->size());
    nextWidget->render(&nextPixmap);
    double value = mCurrentVal.toDouble();
    QRectF r1(value, 0, rect.width() - value, rect.height());
    QRectF r2(0, 0, rect.width() - value, rect.height());
    if(mbIsLeft)
    {
        painter.drawPixmap(r1, nextPixmap, r2);
    }
    else
    {
        painter.drawPixmap(r2, nextPixmap, r1);
    }
}


}
