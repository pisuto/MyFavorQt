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
#include "../button/own_button.h"
#include "own_fadestackedview.h"
#include "../pagebar/own_pagebar.h"
#include "own_settingview.h"

class QFrame;
class QWidget;
class QSystemTrayIcon;

namespace mf {

class OwnMainWidget : public QFrame
{
public:
    Q_OBJECT
public:
    explicit OwnMainWidget(QWidget *parent = Q_NULLPTR);
    ~OwnMainWidget() override;

    QSize getTopRightDownPos() const { return QSize(mpTopBtns->geometry().x() + mpTopBtns->width(), mpTopBtns->geometry().y() + mpTopBtns->height()); }

private:
    QVBoxLayout* mpMainLayout;
    QHBoxLayout* pTopLayout;
    QGridLayout* pListLayout;
    QListWidget* pImgList;
    QVector<QObject*> vpButtons;
    QVector<QListWidgetItem*> vpImgs;
    QVector<mf::OwnElement*> mvpElements;
    QSystemTrayIcon* mpTrayFunc;

    mf::OwnFadeStackedView* mpStackedView;
    mf::OwnButtonGroup* mpBtnGrp;
    mf::OwnPageBar* mpPageBar;
    mf::OwnTopButtonGroup* mpTopBtns;
    mf::OwnItemUploadView* mpItemViewer;
    mf::OwnSettingView* mpSettingView;
};

}

#endif // OWN_MAINWIDGET_H
