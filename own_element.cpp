#include "own_element.h"
#include "own_config.h"

#include <QLabel>
#include <QGridLayout>
#include <QImage>
#include <QPushButton>
#include <QDebug>

namespace mf {

OwnElement::OwnElement(QString fileName, QString title, QString desc)
    :mTitle(qMove(title)), mDesc(qMove(desc)), mpImg(new QImage), mpLable(new QLabel),
      mpLayout(new QGridLayout)
{
    if(!mpImg->load(qMove(fileName)))
    {
        qDebug() << "[ERROR] load image failed.";
        return;
    }

    auto pPixImg = QPixmap::fromImage(*mpImg);
    auto pixSize = OwnConfig::getInstance()->getElementSize();
    pPixImg = pPixImg.scaled(pixSize.width(), pixSize.height(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
    mpLable->setObjectName("own_element_img");
    mpLable->setPixmap(pPixImg);
    mpLable->setScaledContents(true);
    mpLayout->addWidget(mpLable,0,1,1,1);

    this->setObjectName("own_element"); // 设置某一控件的名字
    this->setFrameShape(StyledPanel);
    this->setFrameShadow(Raised);
    this->setLayout(mpLayout);
}

}
