#include "own_fadestackedview.h"
#include "own_slidestackedview.h"

#include <QPainter>
#include <QPropertyAnimation>
#include <QParallelAnimationGroup>
#include <QGraphicsOpacityEffect>
#include <QDebug>

namespace mf {

OwnFadeStackedView::OwnFadeStackedView(OwnPageBar* pageBar, QWidget* parent) :
    QStackedWidget (parent),
    mpEffect(new QGraphicsOpacityEffect(this)),
    mpAnimeAlpha(new QPropertyAnimation(mpEffect, "opacity")),
    mpAnimePos(new QPropertyAnimation()),
    mpAnimeGrp(new QParallelAnimationGroup()),
    mCurrentVal(0),
    mDuration(450),
    mWidgetCnt(0),
    mNextIdx(0),
    mCurrIdx(0),
    mStartColor(parent ? parent->palette().window().color() : Qt::white),
    mbIsAnimation(false),
    mpPageBar(pageBar)
{
    mpAnimeGrp->addAnimation(mpAnimeAlpha);
    mpAnimeGrp->addAnimation(mpAnimePos);

    connect(mpAnimeAlpha, &QPropertyAnimation::valueChanged, this, &OwnFadeStackedView::valueChangedAnimation);
    connect(mpAnimeAlpha, &QPropertyAnimation::finished, this, &OwnFadeStackedView::animationFininshed);
    if(mpPageBar)
        connect(mpPageBar, &OwnPageBar::currentPageChanged, this, &OwnFadeStackedView::switchPage);
}

void OwnFadeStackedView::switchWidget(int index)
{
    if(mbIsAnimation || index == mNextIdx)
        return;
    startAnimation(index);
}

void OwnFadeStackedView::valueChangedAnimation(QVariant value)
{
    mCurrentVal = value;
    this->update();
}

void OwnFadeStackedView::animationFininshed()
{
    mbIsAnimation = false;
}

void OwnFadeStackedView::paintEvent(QPaintEvent* )
{
    if(!mbIsAnimation)
        return;
}

void OwnFadeStackedView::startAnimation(int index)
{
    // https://doc.qt.io/qt-5/qeasingcurve.html
    mbIsAnimation = true;
    mCurrIdx = mNextIdx;
    mNextIdx = static_cast<quint8>(index);
    this->currentWidget()->hide();
    this->setCurrentIndex(mNextIdx);
    // 初始化子页面
    if(mpPageBar)
    {
        auto pView = qobject_cast<OwnSlideStackedView*>(this->currentWidget());
        pView->setCurrentIndex(0);    // index
        mpPageBar->setCurrentPage(1); // index + 1
    }

    this->widget(mNextIdx)->setGraphicsEffect(mpEffect);
    mpAnimeAlpha->setDuration(mDuration / 2);
    mpAnimeAlpha->setStartValue(0.6);
    mpAnimeAlpha->setEndValue(1);
    mpAnimeAlpha->setEasingCurve(QEasingCurve::InQuint);

    mpAnimePos->setTargetObject(this->currentWidget());
    mpAnimePos->setPropertyName("pos");
    mpAnimePos->setDuration(mDuration);
    mpAnimePos->setStartValue(QPoint(0, this->height() * 1/3));
    mpAnimePos->setEndValue(QPoint(0, 0));
    mpAnimePos->setEasingCurve(QEasingCurve::OutExpo);

    mpAnimeGrp->start();
}

void OwnFadeStackedView::switchPage(int page)
{
    auto pView = qobject_cast<OwnSlideStackedView*>(this->currentWidget());
    if(!pView)
        return;
    pView->nextWidget(page - 1);
}

}
