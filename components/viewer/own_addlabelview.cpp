/**
* This file is part of MyFavor
*
* Copyright (C) 2021-2022 Pisuto.
*
*/

#include "own_addlabelview.h"
#include "ui_OwnAddLabelView.h"

#include <QLabel>
#include <QComboBox>
#include <QFrame>
#include <QLineEdit>
#include <QPushButton>
#include <QListView>

namespace mf {

OwnAddLabelView::OwnAddLabelView(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::OwnAddLabelView),
    mpLayout(new QVBoxLayout)
{
    ui->setupUi(this);

    auto pBtnLayout = new QHBoxLayout;
    auto okBtn = new QPushButton("Apply");
    auto cancelBtn = new QPushButton("Cancel");
    auto okStyle = QString("QPushButton{background-color:white;border-radius:5px;border:2px solid #dcdfe6;}"
                         "QPushButton{font-family:'Microsoft YaHei';font-size:11px;color:rgb(150,150,150);}"
                         "QPushButton:hover{background-color:rgb(235, 235, 235);color:rgb(0,255,255);}"
                         "QPushButton:pressed{background-color:rgb(200, 200, 200);color:rgb(0,255,255);}");
    auto cancelStyle = QString("QPushButton{background-color:white;border-radius:5px;border:2px solid #dcdfe6;}"
                         "QPushButton{font-family:'Microsoft YaHei';font-size:11px;color:rgb(150,150,150);}"
                         "QPushButton:hover{background-color:rgb(235, 235, 235);color:rgb(255,105,180);}"
                         "QPushButton:pressed{background-color:rgb(200, 200, 200);color:rgb(255,105,180);}");
    okBtn->setStyleSheet(okStyle);
    cancelBtn->setStyleSheet(cancelStyle);
    okBtn->setFixedSize(QSize(80, 30));
    cancelBtn->setFixedSize(QSize(80, 30));

    pBtnLayout->addWidget(okBtn);
    pBtnLayout->addWidget(cancelBtn);
    auto pFrame = new QFrame;
    pFrame->setLayout(pBtnLayout);

    mpLayout->addWidget(createDropImageBox());
    mpLayout->addWidget(pFrame);
    this->setLayout(mpLayout);
    this->setFixedSize(QSize(320, 120));

    connect(okBtn, &QPushButton::clicked, this, &OwnAddLabelView::confirm);
    connect(cancelBtn, &QPushButton::clicked, this, &QDialog::close);

    auto winStyle = QString("background-color:white;");
    this->setStyleSheet(winStyle);
}

OwnAddLabelView::~OwnAddLabelView()
{
    delete ui;
}

void OwnAddLabelView::confirm()
{
    emit confirmMsg(mpLineEdit->text(), mpCombox->currentData().toString());
    this->close();
}

QFrame* OwnAddLabelView::createDropImageBox()
{
    auto pLayout = new QHBoxLayout;

    auto categoryLabel = new QLabel("Name");
    mpLineEdit = new QLineEdit(this);
    auto lineStyle = QString("QLineEdit{\
                             background:transparent;\
                             color:black;\
                             border-color:rgb(235,235,235);\
                             border-style:solid;\
                             border-top-width:0px;\
                             border-right-width:0px;\
                             border-bottom-width:3px;\
                             border-left-width:0px;}"
                             "QLineEdit:hover{ \
                             border-color:black;}"\
                             "QLineEdit:pressed{ \
                             border-color:black;\
                             }");
    mpLineEdit->setStyleSheet(lineStyle);

    auto iconLabel = new QLabel("Icon");
    mpCombox = new QComboBox;
    auto list = new QListView;
    mpCombox->setView(list);
    auto container = static_cast<QWidget*>(list->parent()); // QComboBoxPrivateContainer对象
    container->setWindowFlags(container->windowFlags() | Qt::NoDropShadowWindowHint); //成功去掉阴影
    auto comboStyle = QString(
                "QComboBox{\
                color:#666666;\
                padding: 0px 20px 2px 20px;\
                border:2px solid rgb(228,228,228);\
                border-radius:5px;}"
                "QComboBox::drop-down { \
                subcontrol-origin: padding; \
                subcontrol-position: top right;\
                width: 15px;\
                border:none;}"
                "QComboBox::down-arrow { \
                width:18px;height:18px;right:2px;\
                image: url(:/images/windows/down-arrow.png);}"
                "QComboBox QAbstractItemView{ \
                background:rgba(255,255,255,1); \
                border:1px solid rgba(228,228,228,1); \
                border-radius:3px; \
                outline: 0px;}"
                "QComboBox QAbstractItemView::item{\
                weight:40px;height:40px;\
                color:#666666;\
                padding-left:20px;\
                background-color:#FFFFFF;}\
                QComboBox QAbstractItemView::item:hover{\
                background-color:rgb(235,235,235);\
                color:#ffffff;}\
                QComboBox QAbstractItemView::item:selected{\
                background-color:rgb(235,235,235);\
                color:#ffffff;}");
    mpCombox->setIconSize(QSize(45, 45));
    mpCombox->setSizeAdjustPolicy(QComboBox::AdjustToContents);
    mpCombox->addItem(QIcon(":/images/svgtopng/anime.png"), "", ":/images/svgtopng/anime.png");
    mpCombox->addItem(QIcon(":/images/svgtopng/manga.png"), "", ":/images/svgtopng/manga.png");
    mpCombox->addItem(QIcon(":/images/svgtopng/movie.png"), "", ":/images/svgtopng/movie.png");
    mpCombox->addItem(QIcon(":/images/svgtopng/music.png"), "", ":/images/svgtopng/music.png");
    mpCombox->addItem(QIcon(":/images/svgtopng/photos.png"), "", ":/images/svgtopng/photos.png");
    mpCombox->setStyleSheet(comboStyle);

    auto pFrame = new QFrame;
    pLayout->addWidget(categoryLabel);
    pLayout->addWidget(mpLineEdit);
    pLayout->addStretch(1);
    pLayout->addWidget(iconLabel);
    pLayout->addWidget(mpCombox);
    pFrame->setLayout(pLayout);
    return pFrame;
}

}
