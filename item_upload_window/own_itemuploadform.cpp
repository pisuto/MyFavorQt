#include "own_itemuploadform.h"
#include "own_config.h"

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
    mpTitleEdit(new QLineEdit),
    mpAuthorEdit(new QLineEdit),
    mpDescEdit(new QLineEdit),
    mpCategoryEdit(new QComboBox),
    mpYearEdit(new QLineEdit),
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
    mpCategoryEdit->addItem(
                SQL_TABLE_ITEM::CategoryToString(SQL_ITEM_CATEGORY::ANIME));
    mpCategoryEdit->addItem(
                SQL_TABLE_ITEM::CategoryToString(SQL_ITEM_CATEGORY::MANGA));
    mpCategoryEdit->addItem(
                SQL_TABLE_ITEM::CategoryToString(SQL_ITEM_CATEGORY::MOVIE));
    mpCategoryEdit->addItem(
                SQL_TABLE_ITEM::CategoryToString(SQL_ITEM_CATEGORY::MUSIC));
    mpCategoryEdit->addItem(
                SQL_TABLE_ITEM::CategoryToString(SQL_ITEM_CATEGORY::PHOTO));

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
            OwnConfig::getInstance()->getMainWindowPtr()->showMinimized();
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
