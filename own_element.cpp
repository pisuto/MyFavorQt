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
#include <QGraphicsOpacityEffect>

namespace mf {

OwnElement::OwnElement(QString fileName, QString title, QString user, QString desc)
    : mpImg(new QImage), mpLable(new QLabel), mpTitle(new QLabel),
      mpUser(new QLabel), mpDesc(new QLabel), mpLayout(new QGridLayout), mpOpacity(new QGraphicsOpacityEffect(this)),
      mTitle(title), mUser(user), mDesc(desc), mIdentification(-1), mbDeleting(false)
{
    initRightMenu(); // 初始化右键选择
    this->setGraphicsEffect(mpOpacity); // 透明化
    auto pConfig = OwnConfig::getInstance();
    auto imgSize = pConfig->getDisplayImageSize();
    int strWidth = static_cast<int>(imgSize.width() * 0.7);
    this->setFixedWidth(imgSize.width() * 1.8);
    this->setFixedHeight(imgSize.height() * 1.1);

    mpLable->setObjectName("own_element_img");
    mpLable->setFixedSize(imgSize);
    this->updateImage(fileName);
    mpLable->setScaledContents(true);
    mpLayout->addWidget(mpLable, 0, 0, 5, 3);

    mpTitle->setObjectName("own_element_title");
    mpUser->setObjectName("own_element_user");
    mpDesc->setObjectName("own_element_desc");
    pConfig->setFont(mpTitle, 15, QFont::Bold);
    pConfig->setFont(mpUser, 10);
    pConfig->setFont(mpDesc, 10);
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

void OwnElement::setElemOpacity(float value)
{
    mpOpacity->setOpacity(qreal(value));
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
