#include "own_element.h"
#include "own_config.h"

#include <QLabel>
#include <QGridLayout>
#include <QImage>
#include <QPushButton>
#include <QDebug>

namespace mf {

OwnElement::OwnElement(QString fileName, QString title, QString user, QString desc)
    :mTitle(title), mUser(user), mDesc(desc), mpImg(new QImage),
      mpLable(new QLabel), mpTitle(new QLabel), mpUser(new QLabel), mpDesc(new QLabel),
      mpLayout(new QGridLayout)
{
    if(!mpImg->load(fileName))
    {
        return;
    }

    auto pPixImg = QPixmap::fromImage(*mpImg);
    auto pixSize = OwnConfig::getInstance()->getElementSize();
    pPixImg = pPixImg.scaled(pixSize.width(), pixSize.height(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
    mpLable->setObjectName("own_element_img");
    mpLable->setPixmap(pPixImg);
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

}
