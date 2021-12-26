#include "own_fadestackedview.h"
#include "own_slidestackedview.h"
#include "own_util.h"

#include <QPropertyAnimation>
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
    if(mpPageBar) /* 最后一个为配置页面，无须设置页码组件 */
    {
        if(index != this->count() - 1) {
            showPageBar();
            mpPageBar->setMaxPage(OwnUtil::getPages(index + 1));
            mpPageBar->setCurrentPage(0, true);
        }
        else {
            hidePageBar();
        }
    }
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
    if(mpPageBar && index != this->count() - 1) /* 最后一个为配置页面，无须设置页码组件 */
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
