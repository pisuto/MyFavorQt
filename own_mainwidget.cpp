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
    mpMainLayout(new QVBoxLayout),
    mpTrayFunc(new QSystemTrayIcon(this)),
    mpBtnGrp(new mf::OwnButtonGroup(this)),
    mpTopBtns(new mf::OwnTopButtonGroup(parent)),
    mpItemViewer(new mf::OwnItemUploadView(this)),
    mpSettingView(new mf::OwnSettingView(this))
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

    // 创建buttons
    mpBtnGrp->setObjectName("own_buttongroup");

    const auto& setting = pConfig->getSettingData();
    const auto& categories = setting.category.categories;
    const auto last = setting.btngrp.btns.size() - 1;
    /* 最起码有一个可用按钮，否则直接退出 */
    if(last <= 2)
    {
        QApplication::quit();
        return;
    }
    const auto& defSize = setting.btngrp.btns[1].size;
    mpBtnGrp->setNormalButtonSize(QSize(static_cast<int>(defSize.width), static_cast<int>(defSize.height)));
    for(size_t index = 0; index <= last; ++index)
    {
        mf::OwnButton* btn = Q_NULLPTR;
        const auto& btnData = setting.btngrp.btns[index];
        if(index == 0) {
            btn = new mf::OwnButton(btnData.id, QIcon(":/images/svgtopng/undo.png"), "");
            btn->setButtonStyle(btnData.font.extent); /* 仅改变字体大小 */
            mpBtnGrp->setOffset(setting.btngrp.offset);
            /* 界面返回 */
        }
        else if(index == last) {
            mpBtnGrp->addStretch(1);
            btn = new mf::OwnButton(btnData.id, QIcon(":/images/svgtopng/setting.png"), "");
            btn->setButtonStyle(btnData.font.extent);
            /* 切换至配置页面 */
            connect(btn, &QPushButton::clicked,
                    this, [&](){this->mpStackedView->switchWidget(btnData.id);});
        }
        else {
            auto category = categories[index - 1];
            btn = new mf::OwnButton(btnData.id, QIcon(QString(category.path.c_str())),
                                         category.name.c_str());
            btn->setButtonStyle(btnData.font.extent);
            connect(btn, &QPushButton::clicked,
                    this, [&](){this->mpStackedView->switchWidget(btnData.id);});
        }
        btn->setIconSize(QSize(static_cast<int>(btnData.icon.width), static_cast<int>(btnData.icon.height)));
        btn->setFixedSize(static_cast<int>(btnData.size.width), static_cast<int>(btnData.size.height));
        if(index == 0) {
            mpBtnGrp->layout()->addWidget(btn);
        }
        else {
            mpBtnGrp->addButton(btn);
        }
    }
    mpBtnGrp->initButtonConnect();

    mpPageBar = new mf::OwnPageBar(OwnUtil::getPages(1));
    mpStackedView = new mf::OwnFadeStackedView(mpPageBar, this);
    auto count = static_cast<int>(categories.size());
    for(int i = 0; i < count; i++)
    {
        auto view = new mf::OwnSlideStackedView(i + 1, mpStackedView);
        OwnUtil::addPages(view, i + 1);
        mpStackedView->addWidget(view);
    }
    mpStackedView->addWidget(mpSettingView); /* 最后的配置命令 */

    // 设置边框
    mpMainLayout->setContentsMargins(5, 5, 5, 5);
    // 设置所有layout之间的包含关系
    mpMainLayout->addWidget(mpTopBtns);
    mpMainLayout->addWidget(mpBtnGrp);
    mpMainLayout->addWidget(mpStackedView);
    mpMainLayout->addWidget(mpPageBar);
    this->setLayout(mpMainLayout);
}

OwnMainWidget::~OwnMainWidget()
{
    // 保存配置
    OwnConfig::getInstance()->save();
}

}
