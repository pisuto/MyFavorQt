#ifndef OWN_MAINWIDGET_H
#define OWN_MAINWIDGET_H

#include <QWidget>
#include <QMainWindow>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QVector>
#include <QListWidget>
#include <QLabel>

#include "own_element.h"
#include "own_slidestackedview.h"
#include "own_button.h"
#include "own_fadestackedview.h"
#include "own_pagebar.h"

class QFrame;
class QWidget;

namespace mf {

class OwnMainWidget : public QFrame
{
public:
    Q_OBJECT
public:
    explicit OwnMainWidget(QWidget *parent = nullptr);
    ~OwnMainWidget() override {}

    QSize getTopRightDownPos() const { return QSize(mpTopBtns->geometry().x() + mpTopBtns->width(), mpTopBtns->geometry().y() + mpTopBtns->height()); }

private:
    QVBoxLayout* pMainLayout;
    QHBoxLayout* pTopLayout;
    QGridLayout* pListLayout;

    QListWidget* pImgList;
    QVector<QObject*> vpButtons;
    QVector<QListWidgetItem*> vpImgs;

    QVector<mf::OwnElement*> mvpElements;
    mf::OwnFadeStackedView* mpStackedView;
    mf::OwnButtonGroup* mpBtnGrp;
    mf::OwnPageBar* mpPageBar;

    mf::OwnTopButtonGroup* mpTopBtns;
};

}

#endif // OWN_MAINWIDGET_H
