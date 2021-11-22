#include "own_element.h"
#include "own_config.h"
#include "item_upload_window/own_itemuploadview.h"
#include "own_database_item.h"
#include "own_util.h"

#include <QLabel>
#include <QGridLayout>
#include <QImage>
#include <QPushButton>
#include <QDebug>
#include <QAction>

namespace mf {

OwnElement::OwnElement(QString fileName, QString title, QString user, QString desc)
    : mpImg(new QImage), mpLable(new QLabel), mpTitle(new QLabel),
      mpUser(new QLabel), mpDesc(new QLabel), mpLayout(new QGridLayout),
      mTitle(title), mUser(user), mDesc(desc), mIdentification(-1), mbDeleting(false)
{
    initRightMenu(); // 初始化右键选择

    mpLable->setObjectName("own_element_img");
    this->updateImage(fileName);
    mpLable->setScaledContents(true);
    mpLayout->addWidget(mpLable, 0, 0, 1, 1);

    mpTitle->setObjectName("own_element_title");
    mpUser->setObjectName("own_element_user");
    mpDesc->setObjectName("own_element_desc");
    mpTitle->setText(mTitle);
    mpUser->setText(mUser);
    mpDesc->setText(mDesc);
    OwnConfig::getInstance()->setTitleStyle(mpTitle);
    mpLayout->addWidget(mpTitle, 1, 0, 1, 1);
    mpLayout->addWidget(mpUser, 2, 0, 1, 1);
    mpLayout->addWidget(mpDesc, 3, 0, 1, 1);

    this->setObjectName("own_element"); // 设置某一控件的名字
    this->setFrameShape(StyledPanel);
    this->setFrameShadow(Raised);
    this->setLayout(mpLayout);
}

void OwnElement::initRightMenu()
{
    auto pDialog = OwnConfig::getInstance()->getItemViewer();
    {
        auto action = new QAction(this);
        action->setText("add");
        connect(action, static_cast<void(QAction::*)(bool)>(&QAction::triggered),
                this, [=](){ pDialog->clear(); pDialog->open(); });
        this->addAction(action);
    }
    {
        auto action = new QAction(this);
        action->setText("update");
        connect(action, static_cast<void(QAction::*)(bool)>(&QAction::triggered),
                this, [=](){ pDialog->openThisItem(this->getId()); pDialog->open(); });
        this->addAction(action);
    }
    {
        auto action = new QAction(this);
        action->setText("delete");
        connect(action, static_cast<void(QAction::*)(bool)>(&QAction::triggered),
                this, [=](){ pDialog->delItem(this->getId()); /* 借用该窗口发送删除消息 */ });
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
    this->setId(item.id.value());
    this->mpTitle->setText(item.title.value());
    this->mpUser->setText(item.author.value());
    this->mpDesc->setText(item.desc.value());
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
