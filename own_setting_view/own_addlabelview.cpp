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
    pBtnLayout->addWidget(okBtn);
    pBtnLayout->addWidget(cancelBtn);
    auto pFrame = new QFrame;
    pFrame->setLayout(pBtnLayout);

    mpLayout->addWidget(createDropImageBox());
    mpLayout->addWidget(pFrame);
    this->setLayout(mpLayout);
    this->setFixedSize(QSize(320, 120));

    connect(okBtn, &QPushButton::clicked, this, [=]() { this->close(); });
    connect(cancelBtn, &QPushButton::clicked, this, &QDialog::close);
}

OwnAddLabelView::~OwnAddLabelView()
{
    delete ui;
}

QFrame* OwnAddLabelView::createDropImageBox()
{
    auto pLayout = new QHBoxLayout;

    auto categoryLabel = new QLabel("Name");
    auto edit = new QLineEdit(this);

    auto iconLabel = new QLabel("Icon");
    auto combox = new QComboBox;
    combox->setView(new QListView);
    auto style = QString(
                "QComboBox{\
                color:#666666;\
                padding: 1px 15px 1px 3px;\
                border:1px solid rgba(228,228,228,1);\
                border-radius:5px 5px 0px 0px;}"
                "QComboBox::drop-down { \
                subcontrol-origin: padding; \
                subcontrol-position: top right;\
                width: 15px;\
                border:none;}"
                "QComboBox::down-arrow { \
                width:18px;height:18px;\
                image: url(:/images/windows/down-arrow.png);}"
                "QComboBox QAbstractItemView{ \
                background:rgba(255,255,255,1); \
                border:1px solid rgba(228,228,228,1); \
                border-radius:0px 0px 5px 5px; \
                outline: 0px;}"
                "QComboBox QAbstractItemView::item{\
                weight:40px;height:40px;\
                color:#666666;\
                padding-left:9px;\
                background-color:#FFFFFF;}\
                QComboBox QAbstractItemView::item:hover{\
                background-color:#409CE1;\
                color:#ffffff;}\
                QComboBox QAbstractItemView::item:selected{\
                background-color:#409CE1;\
                color:#ffffff;}");
    combox->setIconSize(QSize(40, 40));
    combox->setSizeAdjustPolicy(QComboBox::AdjustToContents);
    combox->addItem(QIcon(":/images/svgtopng/anime.png"), "");
    combox->addItem(QIcon(":/images/svgtopng/manga.png"), "");
    combox->addItem(QIcon(":/images/svgtopng/movie.png"), "");
    combox->addItem(QIcon(":/images/svgtopng/music.png"), "");
    combox->addItem(QIcon(":/images/svgtopng/photos.png"), "");
    combox->setStyleSheet(style);

    auto pFrame = new QFrame;
    pLayout->addWidget(categoryLabel);
    pLayout->addWidget(edit);
    pLayout->addStretch(1);
    pLayout->addWidget(iconLabel);
    pLayout->addWidget(combox);
    pFrame->setLayout(pLayout);
    return pFrame;
}

}
