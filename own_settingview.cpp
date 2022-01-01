#include "own_settingview.h"
#include "own_config.h"
#include "own_setting_view/own_aboutview.h"

#include <QSplitter>

namespace mf {

OwnSettingView::OwnSettingView(QWidget *parent) :
    QFrame(parent),
    mpMainLayout(new QVBoxLayout),
    mpBtnGrp(new OwnButtonGroup(this, BTN_GRP_LAYOUT::VERTICAL)),
    mpStackedView(new OwnFadeStackedView(Q_NULLPTR, this))
{
    /* system:
     *          是否最小化至托盘，用打勾按钮
     *          是否使用默认配置，按钮点击
     *          背景颜色设置
     *          分辨率设置，使用下拉框选择
     *          category字体大小设置
     *          增删category，使用滑动列表，点击增加按钮弹出窗口
     * uint:
     *          标题字体大小设置
     *          其余字体大小设置
     *          图片保存路径，点击输入
     * about:
     *          作者、Github、Gitee、CSDN
     *
     */
    const auto& setting = OwnConfig::getInstance()->getSettingData();
    const int last = static_cast<int>(setting.setting.btngrp.btns.size()) - 1;
    if(last < 0) return;
    const auto& defBtn = setting.setting.btngrp.btns[0];
    mpBtnGrp->setNormalButtonSize(QSize(static_cast<int>(defBtn.size.width), static_cast<int>(defBtn.size.height)));
    for(size_t index = 0; index <= last; ++index)
    {
        const auto& btnData = setting.setting.btngrp.btns[index];
        auto btn = new mf::OwnButton(btnData.id, QString(btnData.font.key.c_str()));
        btn->setFixedSize(QSize(static_cast<int>(btnData.size.width), static_cast<int>(btnData.size.height)));
        const auto& normal = btnData.colors[0];
        const auto& hover = btnData.colors[1];
        const auto& pressed = btnData.colors[2];
        btn->setButtonStyle(btnData.font.extent, qRgba(normal.red, normal.green, normal.blue, normal.alpha),
                                                 qRgba(hover.red, hover.green, hover.blue, normal.alpha),
                                                 qRgba(pressed.red, pressed.green, pressed.blue, pressed.alpha),
                            btnData.font.name.c_str());
        connect(btn, &QPushButton::clicked,
                this, [&](){this->mpStackedView->switchWidget(btnData.id);});
        mpBtnGrp->addButton(btn);
    }
    mpBtnGrp->setFixedWidth(static_cast<int>(defBtn.size.width * 1.15)); /* 使得分割线远离一些 */
    mpBtnGrp->addStretch(1);
    mpBtnGrp->initButtonConnect();

    mpStackedView->addWidget(new OwnSystemView(static_cast<int>(defBtn.size.height) / 4,
                                              QFont(defBtn.font.name.c_str(),
                                                    defBtn.font.extent,
                                                    defBtn.font.weight), this));
    mpStackedView->addWidget(new QFrame);
    mpStackedView->addWidget(new OwnAboutView(static_cast<int>(defBtn.size.height),
                                              QFont(defBtn.font.name.c_str(),
                                                    defBtn.font.extent,
                                                    defBtn.font.weight), this));


    auto pSplitter = new QSplitter(Qt::Horizontal);
    pSplitter->setHandleWidth(1);
    pSplitter->setStyleSheet("QSplitter::handle { background-color: grey }");
    pSplitter->addWidget(mpBtnGrp);
    pSplitter->addWidget(mpStackedView);
    pSplitter->handle(1)->setCursor(Qt::ArrowCursor); /* 设置其永远为鼠标格式不再移动 */

    mpMainLayout->setContentsMargins(100, 20, 50, 10);
    mpMainLayout->addWidget(pSplitter);
    this->setLayout(mpMainLayout);
}


}
