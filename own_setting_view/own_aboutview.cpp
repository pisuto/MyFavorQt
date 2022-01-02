#include "own_aboutview.h"
#include "own_button.h"
#include "own_config.h"
#include "own_multilabels.h"

#include <QSplitter>
#include <QLabel>
#include <QComboBox>
#include <QListView>
#include <QMenu>
#include <QWidgetAction>
#include <QProcess>
#include <QMessageBox>

namespace mf {

OwnBublePanel::OwnBublePanel(QWidget *parent) : QFrame(parent),
    mpSplitter(new QSplitter(Qt::Vertical))
{
    this->setAttribute(Qt::WA_TranslucentBackground);//设置背景透明
    this->setWindowFlags(Qt::FramelessWindowHint);
    this->setStyleSheet("QFrame{background-color:rgb(245,245,245);border:none;border-radius:8px;}");

    mpSplitter->setHandleWidth(1);
    mpSplitter->setStyleSheet("QSplitter::handle { background-color: rgba(200,200,200,80) }");
    auto pLayout = new QVBoxLayout();
    pLayout->addWidget(mpSplitter);
    pLayout->setContentsMargins(5, 5, 5, 5);
    this->setLayout(pLayout);
}

void OwnBublePanel::addWidget(QWidget* pItem)
{
    mpSplitter->addWidget(pItem);
}

void OwnBublePanel::finished()
{
    for (int i = 0; i < mpSplitter->count(); i++)
    {
        auto hndl = mpSplitter->handle(i);
        hndl->setCursor(Qt::ArrowCursor);
    }
}

QFrame* createInfoLabel(const QFont& font, const QString& prefix, const QString& link, float scale1 = 0.5f, float scale2 = 0.5f)
{
    auto pLayout = new QVBoxLayout();
    auto prefixLabel = new QLabel(prefix);
    prefixLabel->setFont(QFont(font.family(), static_cast<int>(font.pointSize() * scale1), QFont::Bold));
    pLayout->addWidget(prefixLabel);

    if(!link.isEmpty()) {
        auto linkLabel = new QLabel(link);
        linkLabel->setFont(QFont(font.family(), static_cast<int>(font.pointSize() * scale2)));
        pLayout->addWidget(linkLabel);
    }

    auto pFrame = new QFrame();
    pFrame->setLayout(pLayout);
    return pFrame;
}

QFrame* OwnSystemView::createApplyButton(int size, const QFont& font, const QString& prefix, float scale)
{
    auto pLayout = new QHBoxLayout();
    auto prefixLabel = new QLabel(prefix);
    prefixLabel->setFont(QFont(font.family(), static_cast<int>(font.pointSize() * scale), QFont::Normal));
    auto button = new QPushButton(QIcon(":/images/windows/right-arrow.png"), "");
    button->setFixedSize(size, size);
    button->setIconSize(button->size() * 0.8);
    auto style = QString("QPushButton{background-color:rgb(245, 245, 245);border-radius:5px;}"
                         "QPushButton:hover{background-color:rgb(235, 235, 235);}"
                         "QPushButton:pressed{background-color:rgb(200, 200, 200);}");
    button->setStyleSheet(style);
    connect(button, &QPushButton::clicked, this, &OwnSystemView::recvMsgFromApplyButton);

    pLayout->addWidget(prefixLabel);
    pLayout->addStretch(1);
    pLayout->addWidget(button);
    auto pFrame = new QFrame;
    pFrame->setLayout(pLayout);
    return pFrame;
}

QFrame* OwnSystemView::createToggleButton(int thumb, int track, const QFont& font, const QString& prefix, float scale)
{
    auto pLayout = new QHBoxLayout();
    auto prefixLabel = new QLabel(prefix);
    prefixLabel->setFont(QFont(font.family(), static_cast<int>(font.pointSize() * scale), QFont::Normal));
    auto button = new OwnToggleButton(track, thumb);
    button->setChecked(OwnConfig::getInstance()->getSettingData().trayed);
    OwnConfig::getInstance()->handler(button, HANDLER_OPER::OPER_ADD);
    connect(button, static_cast<void(OwnToggleButton::*)(bool)>(&OwnToggleButton::toggle), this,
            [=](bool checked) { auto& data = OwnConfig::getInstance()->getSettingData();
                                data.trayed = checked; data.modified = true; });

    pLayout->addWidget(prefixLabel);
    pLayout->addStretch(1);
    pLayout->addWidget(button);
    auto pFrame = new QFrame;
    pFrame->setLayout(pLayout);
    return pFrame;
}

QFrame* OwnSystemView::createColorCombox(const QFont& font, const QString& prefix, float scale)
{
    auto pLayout = new QHBoxLayout();
    auto prefixLabel = new QLabel(prefix);
    prefixLabel->setFont(QFont(font.family(), static_cast<int>(font.pointSize() * scale), QFont::Normal));
    auto button = new OwnColorCombox;
    connect(button, SIGNAL(colorChanged(QColor)), this, SLOT(recvMsgFromColorBox(QColor)));

    pLayout->addWidget(prefixLabel);
    pLayout->addStretch(1);
    pLayout->addWidget(button);
    auto pFrame = new QFrame;
    pFrame->setLayout(pLayout);
    return pFrame;
}

QFrame* OwnSystemView::createDropCombox(const QFont& font, const QString& prefix, float scale)
{
    const auto& setting = OwnConfig::getInstance()->getSettingData();
    const auto& screen = setting.screen;
    auto pLayout = new QHBoxLayout();
    auto prefixLabel = new QLabel(prefix);
    prefixLabel->setFont(QFont(font.family(), static_cast<int>(font.pointSize() * scale), QFont::Normal));

    auto button = new QPushButton();
    const auto& defReso = screen.resogrp[screen.index];
    auto defStr = QString("%1×%2").arg(defReso.size.width).arg(defReso.size.height);
    button->setText(defStr);
    button->setFixedSize(90, 30);
    auto btnStyle = QString("QPushButton::menu-indicator{image:url(:/images/windows/down-arrow.png);"
                            "width:18px;height:18px;subcontrol-position:right center;position: absolute;right:2px;}"
                            "QPushButton{font-family:'Microsoft YaHei';font-size:12px;color:rgb(150,150,150);}"
                            "QPushButton{background-color:rgb(245, 245, 245);border:2px solid #dcdfe6;border-radius:5px}");
    auto menu = new QMenu(button);
    for(const auto& reso : screen.resogrp) {
        auto str = QString("%1×%2").arg(reso.size.width).arg(reso.size.height);
        auto pAction = new QAction(str);
        connect(pAction, static_cast<void(QAction::*)(bool)>(&QAction::triggered),
                this, [=](){ button->setText(pAction->text());
                             auto& screen = OwnConfig::getInstance()->getSettingData().screen;
                             for(size_t index = 0; index < screen.resogrp.size(); ++index) {
                                 auto size = screen.resogrp[index].size;
                                 auto str = QString("%1×%2").arg(size.width).arg(size.height);
                                 if(str == pAction->text()) {
                                     screen.index = index;
                                     break;
                                 }
                             }
                             /* 重启 */
                             QMessageBox::StandardButton reply;
                             reply = QMessageBox::question(Q_NULLPTR, "Restart", "Please make sure whether the configuration takes effect at this moment?", QMessageBox::Yes | QMessageBox::No);
                             if(reply == QMessageBox::Yes)
                             {
                                 qApp->quit();
                                 QProcess::startDetached(qApp->arguments()[0], qApp->arguments());
                             }
                           });
        menu->addAction(pAction);
    }
    menu->setAttribute(Qt::WA_TranslucentBackground); //Menu背景透明
    menu->setWindowFlags(menu->windowFlags()  | Qt::FramelessWindowHint | Qt::NoDropShadowWindowHint);
    auto menuStyle = QString("QMenu{font-size:12px;color:rgb(150,150,150);background-color:white;border-radius:5px;"
                             "border:2px solid #dcdfe6;;padding:5px 4px 5px 4px;margin:1px;}"
                             "QMenu::item{margin:2px 5px 2px 5px;}"
                             "QMenu::item:text{padding-left:2px;padding-right:2px;}"
                             "QMenu::item:selected{color:black;background-color:rgb(235,235,235);border-radius:3px;}"
                             "QMenu::separator{height:1px;background:#bbbbbb;margin-left:2px;margin-right:2px;}");
    menu->setStyleSheet(menuStyle);
    menu->setCursor(QCursor(Qt::PointingHandCursor));
    button->setMenu(menu);
    button->setCursor(QCursor(Qt::PointingHandCursor));
    button->setStyleSheet(btnStyle);

    pLayout->addWidget(prefixLabel);
    pLayout->addStretch(1);
    pLayout->addWidget(button);
    auto pFrame = new QFrame;
    pFrame->setLayout(pLayout);
    return pFrame;
}

QFrame* OwnSystemView::createMultiLabels()
{
    auto pLayout = new QHBoxLayout();
    const auto& setting = OwnConfig::getInstance()->getSettingData();
    const auto& categories = setting.category.categories;

    auto labels = new OwnMultiLabels;
    for(const auto& category : categories) {
        labels->addLabel(new OwnLabel(category.name.c_str()));
    }
    connect(this, SIGNAL(sendMsgToObj(QString, QString)), labels, SLOT(insertLabel(QString, QString)));

    auto button = new QPushButton("Add");
    button->setFixedSize(QSize(90, 30));
    auto style = QString("QPushButton{font-family:'Microsoft YaHei';font-size:12px;color:rgb(150,150,150);}"
                         "QPushButton{background-color:rgb(245, 245, 245);border:2px solid #dcdfe6;border-radius:5px}"
                         "QPushButton:hover{background-color:rgb(235, 235, 235);}"
                         "QPushButton:pressed{background-color:rgb(200, 200, 200);}");
    button->setStyleSheet(style);
    connect(button, &QPushButton::clicked, this, [=]() { this->mpDialog->open(); });

    pLayout->addWidget(labels);
    pLayout->addStretch(1);
    pLayout->addWidget(button);
    auto pFrame = new QFrame;
    pFrame->setLayout(pLayout);
    return pFrame;
}

OwnAboutView::OwnAboutView(int, QFont font, QWidget *parent) : QFrame (parent),
    mpMainLayout(new QVBoxLayout)
{
    auto pPanel = new OwnBublePanel(this);
    pPanel->addWidget(createInfoLabel(font, "Github", "https://github.com/pisuto/MyFavorQt"));
    pPanel->addWidget(createInfoLabel(font, "Gitee", "https://gitee.com/wu_git/MyFavorQt"));
    pPanel->addWidget(createInfoLabel(font, "CSDN Blog", "https://blog.csdn.net/pisuto"));
    pPanel->finished();

    mpMainLayout->setContentsMargins(30, 10, 30, 10);
    /* 标题 */
    mpMainLayout->addWidget(createInfoLabel(font, "About", "For more infomation", 1, 0.5f));
    mpMainLayout->addWidget(pPanel);
    mpMainLayout->addStretch(1);
    this->setLayout(mpMainLayout);
}

OwnSystemView::OwnSystemView(int height, QFont font, QWidget *parent) : QFrame(parent),
   mpDialog(new OwnAddLabelView(this)), mpMainLayout(new QVBoxLayout)
{
    auto pPanel1 = new OwnBublePanel(this);
    pPanel1->addWidget(createToggleButton(height, height * 4 / 5, font, "Minimize to tray by default"));
    pPanel1->addWidget(createApplyButton(height * 3, font, "Restore settings to default values"));
    pPanel1->addWidget(createColorCombox(font, "Choose background color"));
    pPanel1->addWidget(createDropCombox(font, "Choose appropriate resolution"));
    pPanel1->finished();

    auto pPanel2 = new OwnBublePanel(this);
    pPanel2->addWidget(createMultiLabels());
    pPanel1->finished();

    mpMainLayout->setContentsMargins(30, 10, 30, 10);
    mpMainLayout->addWidget(createInfoLabel(font, "System", "Setup configuration and some performance takes effect after restart", 1, 0.5f));
    mpMainLayout->addWidget(createInfoLabel(font, "Basic", "", 0.7f));
    mpMainLayout->addWidget(pPanel1);
    mpMainLayout->addWidget(createInfoLabel(font, "Advanced", "Customize categories", 0.7f));
    mpMainLayout->addWidget(pPanel2);
    mpMainLayout->addWidget(createInfoLabel(font, "Warning: the operation may cause data loss.", "", 0.5f));
    mpMainLayout->addStretch(1);
    this->setLayout(mpMainLayout);

    connect(mpDialog, SIGNAL(confirmMsg(QString, QString)), this, SLOT(recvMsgFromDialog(QString, QString)));
}

void OwnSystemView::recvMsgFromDialog(QString name, QString path)
{
    emit sendMsgToObj(name, path);
}

void OwnSystemView::recvMsgFromColorBox(QColor color)
{
    OwnConfig::getInstance()->handler(this, HANDLER_OPER::OPER_MOD, color);
}

void OwnSystemView::recvMsgFromApplyButton()
{
    OwnConfig::getInstance()->handler(this, HANDLER_OPER::OPER_MOD, true);
}



}
