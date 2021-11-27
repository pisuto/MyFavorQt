#include "own_fadestackedview.h"
#include "own_slidestackedview.h"

#include <QPropertyAnimation>
#include <QParallelAnimationGroup>
#include <QGraphicsOpacityEffect>
#include <QDebug>

namespace mf {

OwnFadeStackedView::OwnFadeStackedView(OwnPageBar* pageBar, QWidget* parent) :
    QStackedWidget (parent),
    mpAnimePos(new QPropertyAnimation()),
    mDuration(400),
    mWidgetCnt(0),
    mNextIdx(0),
    mCurrIdx(0),
    mStartColor(parent ? parent->palette().window().color() : Qt::white),
    mbIsAnimation(false),
    mpPageBar(pageBar)
{
    if(mpPageBar)
        connect(mpPageBar, &OwnPageBar::currentPageChanged, this, &OwnFadeStackedView::switchPage);
}

void OwnFadeStackedView::switchWidget(int index)
{
    if(index == mNextIdx)
        return;
    startAnimation(index);
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

    mpAnimePos->setTargetObject(this->currentWidget());
    mpAnimePos->setPropertyName("pos");
    mpAnimePos->setDuration(mDuration);
    mpAnimePos->setStartValue(QPoint(0, this->height() * 1/4));
    mpAnimePos->setEndValue(QPoint(0, 0));
    mpAnimePos->setEasingCurve(QEasingCurve::OutExpo);
    mpAnimePos->start();
}

void OwnFadeStackedView::switchPage(int page)
{
    auto pView = qobject_cast<OwnSlideStackedView*>(this->currentWidget());
    if(!pView)
        return;
    pView->nextWidget(page - 1);
}

}
