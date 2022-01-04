/**
* This file is part of MyFavor
*
* Copyright (C) 2021-2022 Pisuto.
*
*/

#include "own_element.h"
#include "../global/own_config.h"
#include "../dialog/own_itemuploadview.h"
#include "../global/own_database_item.h"
#include "../global/own_util.h"

#include <QLabel>
#include <QGridLayout>
#include <QImage>
#include <QPushButton>
#include <QDebug>
#include <QAction>
#include <QPainter>

namespace mf {

OwnElement::OwnElement(QString fileName, QString title, QString user, QString desc)
    : mpImg(new QImage), mpLable(new QLabel), mpTitle(new QLabel),
      mpUser(new QLabel), mpDesc(new QLabel), mpLayout(new QGridLayout),
      mTitle(title), mUser(user), mDesc(desc), mIdentification(-1), mbDeleting(false)
{
    // 初始化右键选择
    initRightMenu();
    // hiding widget and keeping widget space
    auto policy = this->sizePolicy();
    policy.setRetainSizeWhenHidden(true);
    this->setSizePolicy(policy);

    auto pConfig = OwnConfig::getInstance();
    auto imgSize = pConfig->getDisplayImageSize();
    int strWidth = static_cast<int>(imgSize.width() * 0.7);
    this->setFixedWidth(imgSize.width() * 1.8);
    this->setFixedHeight(imgSize.height() * 1.1);

    mpLable->setObjectName("own_element_img");
    mpTitle->setObjectName("own_element_title");
    mpUser->setObjectName("own_element_user");
    mpDesc->setObjectName("own_element_desc");
    mpLable->setFixedSize(imgSize);
    this->updateImage(fileName);
    mpLable->setScaledContents(true);
    mpLayout->addWidget(mpLable, 0, 0, 5, 3);
    /* 设置字体 */
    const auto& component = pConfig->getSettingData().element;
    const auto& fonts = component.fonts;
    mpTitle->setFont(QFont(fonts[0].name.c_str(), fonts[0].extent, fonts[0].weight));
    mpUser->setFont(QFont(fonts[1].name.c_str(), fonts[1].extent, fonts[1].weight));
    mpDesc->setFont(QFont(fonts[2].name.c_str(), fonts[2].extent, fonts[2].weight));

    mpTitle->setText(OwnUtil::strAutoFeed(mTitle, mpTitle->font(), 3, strWidth));
    mpUser->setText(OwnUtil::strAutoFeed(mUser, mpUser->font(), 2, strWidth));
    mpDesc->setText(OwnUtil::strAutoFeed(mDesc, mpDesc->font(), 6, strWidth));
    mpLayout->addWidget(mpTitle, 0, 4, 1, 4);
    mpLayout->addWidget(mpUser, 1, 4, 1, 4);
    mpLayout->addWidget(mpDesc, 2, 4, 1, 4);

    this->setObjectName("own_element"); // 设置某一控件的名字
    this->setFrameShape(StyledPanel);
    this->setFrameShadow(Raised);
    this->setLayout(mpLayout);
    this->setStyleSheet("QFrame#own_element{background-color:rgb(245, 245, 245);}");
}

void OwnElement::initRightMenu()
{
    auto pDialog = OwnConfig::getInstance()->getItemViewer();
    {
        auto action = new QAction("add", this);
        connect(action, static_cast<void(QAction::*)(bool)>(&QAction::triggered),
                this, [=](){ pDialog->clear(); pDialog->open(); });
        this->addAction(action);
    }
    {
        auto action = new QAction("update", this);
        connect(action, static_cast<void(QAction::*)(bool)>(&QAction::triggered),
                this, [=](){ pDialog->openThisItem(this->getId()); pDialog->open(); });
        this->addAction(action);
    }
    {
        auto action = new QAction("delete", this);
        connect(action, static_cast<void(QAction::*)(bool)>(&QAction::triggered),
                this, [=](){ pDialog->delItem(this->getId()); /* 借用该窗口发送删除消息 */ });
        this->addAction(action);
    }
    {
        auto action = new QAction(this);
        action->setText("restore");
        connect(action, static_cast<void(QAction::*)(bool)>(&QAction::triggered),
                this,
                [=](){ this->updateImage(*this->mpImg);
                       this->setDeletingStatus(false);
                       OwnDatabase::getInstance()->removeDeletingItem(this->getId());});
        this->addAction(action);
    }

    this->setContextMenuPolicy(Qt::ActionsContextMenu);
}

void OwnElement::updateImage(QString fileName)
{
    if(!mpImg->load(fileName))
    {
        return;
    }
    updateImage(*mpImg);
}

void OwnElement::updateImage(QImage &tmpImg)
{
    auto pPixImg = QPixmap::fromImage(tmpImg);
    auto pixSize = OwnConfig::getInstance()->getDisplayImageSize();
    pPixImg = pPixImg.scaled(pixSize.width(), pixSize.height(),
                             Qt::KeepAspectRatio, Qt::SmoothTransformation);
    mpLable->setPixmap(pPixImg);
}

bool OwnElement::getDeletingStatus() const
{
    return mbDeleting;
}

void OwnElement::setDeletingStatus(bool value)
{
    mbDeleting = value;
}

void OwnElement::setId(int id)
{
    mIdentification = id;
}

int OwnElement::getId() const
{
    return mIdentification;
}

void OwnElement::setCategory(int category)
{
    mCategory = category;
}

int OwnElement::getCategory() const
{
    return mCategory;
}

void OwnElement::setValue(odbitem &item)
{
    auto imgSize = OwnConfig::getInstance()->getDisplayImageSize();
    int strWidth = static_cast<int>(imgSize.width() * 0.7);

    this->setId(item.id.value());
    this->mpTitle->setText(OwnUtil::strAutoFeed(item.title.value(), mpTitle->font(), 3, strWidth));
    this->mpUser->setText(OwnUtil::strAutoFeed(item.author.value(), mpUser->font(), 2, strWidth));
    this->mpDesc->setText(OwnUtil::strAutoFeed(item.desc.value(), mpDesc->font(), strWidth));
    this->setCategory(item.category.value());

    QString imgPath = SQL_TABLE_ITEM::ImgFileLocation() + "/" +
            QString::number(this->getId()) + ".png";
    this->updateImage(imgPath);
}

void OwnElement::reset()
{

}

bool OwnElement::deleting()
{
    QImage overlay;
    if(!overlay.load(":/images/svgtopng/deleted.png"))
    {
        return false;
    }

    auto delImg = OwnUtil::getImgWithOverlay(*mpImg, overlay);
    updateImage(delImg);
    setDeletingStatus(true);
    return getDeletingStatus();
}

}
