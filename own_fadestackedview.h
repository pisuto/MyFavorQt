#ifndef OWN_FADESTACKEDVIEW_H
#define OWN_FADESTACKEDVIEW_H

#include <QStackedWidget>
#include <QVariant>

#include "own_pagebar.h"

class QPropertyAnimation;

namespace mf {

class OwnFadeStackedView : public QStackedWidget
{
    Q_OBJECT
public:
    explicit OwnFadeStackedView(OwnPageBar* pageBar, QWidget* parent = nullptr);
    ~OwnFadeStackedView() {}

    void initAnimationEffect();
    void switchWidget(int index);
    void setDuration(quint16 dura) { mDuration = dura; }
    OwnPageBar* getPageBar() { return mpPageBar; }
    void hidePageBar() { if(mpPageBar) mpPageBar->hide(); }
    void showPageBar() { if(mpPageBar) mpPageBar->show(); }

private:
    void startAnimation(int index);
    void switchPage(int page);

private:
    QPropertyAnimation* mpAnimePos;
    quint16 mDuration;
    quint8 mWidgetCnt;
    quint8 mNextIdx;
    quint8 mCurrIdx;
    bool mbIsAnimation;

    // 切换页面
    OwnPageBar* mpPageBar;
};

}

#endif // OWN_FADESTACKEDVIEW_H
