/**
* This file is part of MyFavor
*
* Copyright (C) 2021-2022 Pisuto.
*
*/

#include "own_itemuploadform.h"
#include "../global/own_config.h"

#include <QVBoxLayout>
#include <QLineEdit>
#include <QLabel>
#include <QComboBox>
#include <QPushButton>
#include <QGroupBox>
#include <QDialog>
#include <QFileDialog>
#include <QMessageBox>

namespace mf {

OwnItemUploadForm::OwnItemUploadForm(QWidget* parent) : QFrame(parent),
    mpLayout(new QVBoxLayout),
    mpTitle(new QLabel),
    mpAuthor(new QLabel),
    mpDesc(new QLabel),
    mpCategory(new QLabel),
    mpYear(new QLabel),
    mpTitleEdit(new OwnLineEdit),
    mpAuthorEdit(new OwnLineEdit),
    mpDescEdit(new OwnLineEdit),
    mpCategoryEdit(new QComboBox),
    mpYearEdit(new OwnLineEdit),
    mpCrop(new QLabel),
    mpStartCapBtn(new QPushButton),
    mpUploadImgBtn(new QPushButton),
    mpApplyBtn(new QPushButton),
    mpViewer(new OwnImageViewer(mpCrop, this)),
    mpCaptrue(new OwnScreenCapture(mpViewer)),
    mpParent(qobject_cast<QDialog*>(parent))
{
    mpTitle->setText("Title");
    mpAuthor->setText("Author");
    mpDesc->setText("Description");
    mpCategory->setText("Category");
    mpYear->setText("Year");
    mpStartCapBtn->setText("Capture");
    mpUploadImgBtn->setText("Upload");
    mpApplyBtn->setText("Apply");

    mpCrop->setFixedSize(OwnConfig::getInstance()->getDisplayImageSize());
    mpYearEdit->setValidator(new QIntValidator(0, 3000, this)); /* 只能输入数字 */
    mpCategoryEdit->clear();

    const auto& categories = OwnConfig::getInstance()->getSettingData().category.categories;
    for(const auto& info : categories) {
        mpCategoryEdit->addItem(info.name.c_str());
    }

    mpLayout->addWidget(mpTitle);
    mpLayout->addWidget(mpTitleEdit);
    mpLayout->addWidget(mpAuthor);
    mpLayout->addWidget(mpAuthorEdit);
    mpLayout->addWidget(mpDesc);
    mpLayout->addWidget(mpDescEdit);
    mpLayout->addWidget(mpCategory);
    mpLayout->addWidget(mpCategoryEdit);
    mpLayout->addWidget(mpYear);
    mpLayout->addWidget(mpYearEdit);

    auto pHBox1 = new QGridLayout();
    auto pGrpBox1 = new QGroupBox("Image");
    pHBox1->addWidget(mpViewer);
    pGrpBox1->setLayout(pHBox1);

    auto pHBox2 = new QGridLayout();
    auto pGrpBox2 = new QGroupBox("Applyed");
    pHBox2->addWidget(mpCrop);
    pGrpBox2->setLayout(pHBox2);
//    mpLayout->addWidget(mpViewer, 0, Qt::AlignCenter);

    auto pHBox3 = new QHBoxLayout();
    pHBox3->addWidget(mpStartCapBtn);
    pHBox3->addWidget(mpUploadImgBtn);
    pHBox3->addWidget(mpApplyBtn);
    pHBox3->setContentsMargins(0, 0, 0, 0);

    auto pHBox4 = new QHBoxLayout();
    pHBox4->addWidget(pGrpBox1);
    pHBox4->addWidget(pGrpBox2);

    mpLayout->addLayout(pHBox4);
    mpLayout->addLayout(pHBox3);
    this->setLayout(mpLayout);

    connect(mpStartCapBtn, &QPushButton::clicked, this, [&](){
        if(this->mpParent)
        {
            OwnConfig::getInstance()->handler(static_cast<MainWindow*>(Q_NULLPTR), HANDLER_OPER::OPER_MIN);
            this->mpParent->showMinimized();
        }
        this->mpCaptrue->showFullScreen();});
    connect(mpUploadImgBtn, &QPushButton::clicked, this, [&](){
        this->chooseImageDialog();
    });
    connect(mpApplyBtn, &QPushButton::clicked, this, [&](){
        this->mpViewer->cropFinished();
    });
}

QWidget *OwnItemUploadForm::getFormInfoByTag(const QString &tag, SQL_ITEM_OPER oper)
{
    if(tag == SQL_TABLE_ITEM::ITEM_TITLE)
    {
        if(mpTitleEdit->text().isEmpty() && oper == SQL_ITEM_OPER::INSERT)
        {
            mpTitleEdit->setNullStyle();
            return Q_NULLPTR;
        }
        return mpTitleEdit;
    }
    else if(tag == SQL_TABLE_ITEM::ITEM_AUTHOR)
    {
        if(mpAuthorEdit->text().isEmpty() && oper == SQL_ITEM_OPER::INSERT)
        {
            mpAuthorEdit->setNullStyle();
            return Q_NULLPTR;
        }
        return mpAuthorEdit;
    }
    else if(tag == SQL_TABLE_ITEM::ITEM_DESC)
    {
        if(mpDescEdit->text().isEmpty() && oper == SQL_ITEM_OPER::INSERT)
        {
            mpDescEdit->setNullStyle();
            return Q_NULLPTR;
        }
        return mpDescEdit;
    }
    else if(tag == SQL_TABLE_ITEM::ITEM_IMAGEPATH)
    {
        return this->mpViewer;
    }
    else if(tag == SQL_TABLE_ITEM::ITEM_CATEGORY)
    {
        return mpCategoryEdit;
    }
    else if(tag == SQL_TABLE_ITEM::ITEM_CRATEYEAR)
    {
        if(mpYearEdit->text().isEmpty() && oper == SQL_ITEM_OPER::INSERT)
        {
            mpYearEdit->setNullStyle();
            return Q_NULLPTR;
        }
        return mpYearEdit;
    }

    return Q_NULLPTR;
}

void OwnItemUploadForm::clearData()
{
    mpTitleEdit->clear();
    mpAuthorEdit->clear();
    mpDescEdit->clear();
    mpCategoryEdit->setCurrentIndex(0);
    mpYearEdit->clear();
    mpViewer->reset();
}

void OwnItemUploadForm::chooseImageDialog()
{
    auto path = QFileDialog::getOpenFileName(this, tr("Choose Image"), ".", tr("Image Files(*.jpg *.png)"));
    auto pImg = new QImage;
    if(!(pImg->load(path)))
    {
        QMessageBox::information(this, tr("Open File"), tr("Loading Image Failed!"));
        delete pImg;
        return;
    }
    this->mpViewer->setPixmap(QPixmap::fromImage(*pImg));
}


}
