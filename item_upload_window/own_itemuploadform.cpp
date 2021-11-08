#include "own_itemuploadform.h"
#include "own_config.h"

#include <QVBoxLayout>
#include <QLineEdit>
#include <QLabel>
#include <QComboBox>
#include <QPushButton>

namespace mf {

OwnItemUploadForm::OwnItemUploadForm(bool screenMethod, QWidget* parent) : QFrame(parent),
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
    mpStartCapBtn(new QPushButton),
    mbImgMethod(screenMethod),
    mpCaptrue(new OwnScreenCapture)
{
    mpTitle->setText("Title");
    mpAuthor->setText("Author");
    mpDesc->setText("Description");
    mpCategory->setText("Category");
    mpYear->setText("Year");
    mpStartCapBtn->setText("Capttrue");

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
    mpLayout->addWidget(mpStartCapBtn);

    this->setLayout(mpLayout);

    connect(mpStartCapBtn, &QPushButton::clicked, this, [&](){this->mpCaptrue->showFullScreen();});
}


}
