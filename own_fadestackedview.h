#ifndef OWN_FADESTACKEDVIEW_H
#define OWN_FADESTACKEDVIEW_H

#include <QStackedWidget>
#include <QVariant>

#include "own_pagebar.h"

class QPropertyAnimation;
class QParallelAnimationGroup;
class QGraphicsOpacityEffect;

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

public slots:
    void valueChangedAnimation(QVariant value);
    void animationFininshed();

protected:
    void paintEvent(QPaintEvent* event);

private:
//    void paintPrevious(QPainter& painter, int index);
//    void paintNext(QPainter& painter, int index);

    void startAnimation(int index);
    void switchPage(int page);

private:
    QGraphicsOpacityEffect* mpEffect;
    QPropertyAnimation* mpAnimeAlpha;
    QPropertyAnimation* mpAnimePos;
    QParallelAnimationGroup* mpAnimeGrp;
    QVariant mCurrentVal;
    quint16 mDuration;
    quint8 mWidgetCnt;
    quint8 mNextIdx;
    quint8 mCurrIdx;
    QColor mStartColor;
    bool mbIsAnimation;

    // 切换页面
    OwnPageBar* mpPageBar;
};

}

#endif // OWN_FADESTACKEDVIEW_H
