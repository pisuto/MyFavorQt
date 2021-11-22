#include "own_mainwidget.h"
#include "own_config.h"
#include "own_database.h"
#include "own_database_item.h"
#include "own_util.h"

#include <QPainter>
#include <QtMath>
#include <QMenu>

namespace mf {

OwnMainWidget::OwnMainWidget(QWidget *parent) :
    QFrame(parent),
    mpBtnGrp(new mf::OwnButtonGroup(40, this)),
    mpTopBtns(new mf::OwnTopButtonGroup(parent))
{
    // 创建layout
    pMainLayout = new QVBoxLayout();
    // 创建buttons
    auto btn1 = new mf::OwnButton(-1, QIcon(":/images/svgtopng/undo.png"), "");
    auto btn2 = new mf::OwnButton(0, QIcon(":/images/svgtopng/anime.png"), "Anime");
    auto btn3 = new mf::OwnButton(1, QIcon(":/images/svgtopng/manga.png"), "Manga");
    auto btn4 = new mf::OwnButton(2, QIcon(":/images/svgtopng/movie.png"), "Movie");
    auto btn5 = new mf::OwnButton(3, QIcon(":/images/svgtopng/music.png"), "Music");
    auto btn6 = new mf::OwnButton(4, QIcon(":/images/svgtopng/photos.png"), "Photo");
    auto btn7 = new mf::OwnButton(5, QIcon(":/images/svgtopng/setting.png"), "");
    btn1->setIconSize(QSize(30,30));
    btn1->setFixedSize(40, 30);
    btn2->setIconSize(QSize(20,20));
    btn3->setIconSize(QSize(20,20));
    btn4->setIconSize(QSize(20,20));
    btn5->setIconSize(QSize(20,20));
    btn6->setIconSize(QSize(20,20));
    btn7->setIconSize(QSize(20,20));
    connect(btn2, &QPushButton::clicked,
            this, [&](){this->mpStackedView->switchWidget(0);});
    connect(btn3, &QPushButton::clicked,
            this, [&](){this->mpStackedView->switchWidget(1);});
    connect(btn4, &QPushButton::clicked,
            this, [&](){this->mpStackedView->switchWidget(2);});
    connect(btn5, &QPushButton::clicked,
            this, [&](){this->mpStackedView->switchWidget(3);});
    connect(btn6, &QPushButton::clicked,
            this, [&](){this->mpStackedView->switchWidget(4);});
    // 添加buttons到layout里
    mpBtnGrp->setObjectName("own_buttongroup");
    mpBtnGrp->layout()->addWidget(btn1);
    mpBtnGrp->addButton(btn2);
    mpBtnGrp->addButton(btn3);
    mpBtnGrp->addButton(btn4);
    mpBtnGrp->addButton(btn5);
    mpBtnGrp->addButton(btn6);
    mpBtnGrp->addStretch(1);
    mpBtnGrp->addButton(btn7);
    mpBtnGrp->initButtonConnect();

    // 初始化页面的分页组件
    mpPageBar = new mf::OwnPageBar(OwnUtil::getPages(1));
    mpStackedView = new mf::OwnFadeStackedView(mpPageBar, this);
    // 显示元素
    int count = static_cast<int>(SQL_ITEM_CATEGORY::COUNT);
    for(int i = 0; i < count; i++)
    {
        auto view = new mf::OwnSlideStackedView(i + 1, mpStackedView);
        OwnUtil::addPages(view, i + 1);
        mpStackedView->addWidget(view);
    }

    // 设置边框
    pMainLayout->setContentsMargins(5, 5, 5, 5);
    // 设置所有layout之间的包含关系
    pMainLayout->addWidget(mpTopBtns);
    pMainLayout->addWidget(mpBtnGrp);
    pMainLayout->addWidget(mpStackedView);
    pMainLayout->addWidget(mpPageBar);
    this->setLayout(pMainLayout);
}

}
