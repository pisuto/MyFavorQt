#include "own_mainwidget.h"
#include "own_config.h"
#include "own_database.h"
#include "own_database_item.h"
#include "own_util.h"

#include <QtMath>
#include <QMenu>
#include <QSystemTrayIcon>

namespace mf {

OwnMainWidget::OwnMainWidget(QWidget *parent) :
    QFrame(parent),
    mpTrayFunc(new QSystemTrayIcon(this)),
    mpBtnGrp(new mf::OwnButtonGroup(this)),
    mpTopBtns(new mf::OwnTopButtonGroup(parent)),
    mpItemViewer(new mf::OwnItemUploadView(this))
{
    // 设置对话框
    auto pConfig = OwnConfig::getInstance();
    pConfig->setItemUploadView(mpItemViewer);
    // 初始化托盘
    mpTrayFunc->setIcon(QIcon(":images/icon/tray.png"));
    mpTrayFunc->setToolTip(tr("MyFavor"));
    connect(mpTrayFunc, static_cast<void(QSystemTrayIcon::*)(QSystemTrayIcon::ActivationReason)>(&QSystemTrayIcon::activated),
            this, [](QSystemTrayIcon::ActivationReason reason)
                  { if(reason == QSystemTrayIcon::DoubleClick) OwnConfig::getInstance()->showWindowFromTray(); });
    auto pTrayMenu = new QMenu(this);
    {
        auto pAction = new QAction(tr("show"), this);
        connect(pAction, static_cast<void(QAction::*)(bool)>(&QAction::triggered),
                this, [](){ OwnConfig::getInstance()->showWindowFromTray(); });
        pTrayMenu->addAction(pAction);
    }
    {
        auto pAction = new QAction(tr("exit"), this);
        connect(pAction, static_cast<void(QAction::*)(bool)>(&QAction::triggered),
                this, [](){ QApplication::quit(); });
        pTrayMenu->addAction(pAction);
    }
    mpTrayFunc->setContextMenu(pTrayMenu);
    connect(mpTopBtns, static_cast<void(OwnTopButtonGroup::*)(bool)>(&OwnTopButtonGroup::closeWindowToTray), this,
            [=](bool){ this->mpTrayFunc->show(); mpTrayFunc->showMessage(windowTitle(), "Here!"); });

    // 创建layout
    pMainLayout = new QVBoxLayout();
    // 创建buttons
    mpBtnGrp->setObjectName("own_buttongroup");
#ifdef AUTO_SETTING_LOAD
    const auto& setting = pConfig->getSettingData();
    const auto& categories = setting.category.categories;
    for(size_t index = 0; index < setting.btngrp.btns.size(); ++index)
    {
        const auto& data = setting.btngrp.btns[index];
        if(index == 0) {
            auto btn = new mf::OwnButton(data.id, QIcon(":/images/svgtopng/undo.png"), "");
            btn->setIconSize(QSize(static_cast<int>(data.icon.width), static_cast<int>(data.icon.height)));
            btn->setFixedSize(static_cast<int>(data.size.width), static_cast<int>(data.size.height));
            mpBtnGrp->setOffset(data.size.width); /* 设置偏移 */
            mpBtnGrp->layout()->addWidget(btn);
            continue;
        }
        auto category = categories[index - 1];
        auto btn = new mf::OwnButton(data.id, QIcon(QString(category.path.c_str())),
                                     category.name.c_str());
        btn->setIconSize(QSize(static_cast<int>(data.icon.width), static_cast<int>(data.icon.height)));
        btn->setFixedSize(static_cast<int>(data.size.width), static_cast<int>(data.size.height));
        connect(btn, &QPushButton::clicked,
                this, [&](){this->mpStackedView->switchWidget(static_cast<int>(index - 1));});
        mpBtnGrp->addButton(btn);
        if(index == 1) {
            mpBtnGrp->setNormalButtonSize(btn->size());
        }
    }
    mpBtnGrp->initButtonConnect();

    mpPageBar = new mf::OwnPageBar(OwnUtil::getPages(1));
    mpStackedView = new mf::OwnFadeStackedView(mpPageBar, this);
    int count = static_cast<int>(categories.size());
    for(int i = 0; i < count; i++)
    {
        auto view = new mf::OwnSlideStackedView(i + 1, mpStackedView);
        OwnUtil::addPages(view, i + 1);
        mpStackedView->addWidget(view);
    }
#else
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
    btn2->setFixedSize(80, 30);
    btn3->setIconSize(QSize(20,20));
    btn3->setFixedSize(80, 30);
    btn4->setIconSize(QSize(20,20));
    btn4->setFixedSize(80, 30);
    btn5->setIconSize(QSize(20,20));
    btn5->setFixedSize(80, 30);
    btn6->setIconSize(QSize(20,20));
    btn6->setFixedSize(80, 30);
    btn7->setIconSize(QSize(20,20));
    btn7->setFixedSize(40, 30);
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
    mpBtnGrp->setNormalButtonSize(QSize(80, 30));
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
#endif
    // 设置边框
    pMainLayout->setContentsMargins(5, 5, 5, 5);
    // 设置所有layout之间的包含关系
    pMainLayout->addWidget(mpTopBtns);
    pMainLayout->addWidget(mpBtnGrp);
    pMainLayout->addWidget(mpStackedView);
    pMainLayout->addWidget(mpPageBar);
    this->setLayout(pMainLayout);
}

OwnMainWidget::~OwnMainWidget()
{
    // 保存配置

}

}
