/**
* This file is part of MyFavor
*
* Copyright (C) 2021-2022 Pisuto.
*
*/

#ifndef OWN_ITEMUPLOADFORM_H
#define OWN_ITEMUPLOADFORM_H

#include <QFrame>

#include "own_screencapture.h"
#include "own_imageviewer.h"
#include "../edit/own_lineedit.h"

class QVBoxLayout;
class QLineEdit;
class QLabel;
class QComboBox;
class QPushButton;

namespace mf {

class OwnItemUploadForm : public QFrame
{
    Q_OBJECT
public:
    explicit OwnItemUploadForm(QWidget* parent = Q_NULLPTR);
    QWidget* getFormInfoByTag(const QString& tag, SQL_ITEM_OPER oper = SQL_ITEM_OPER::INSERT);

    void clearData();

private:
    void chooseImageDialog();

private:
    QVBoxLayout* mpLayout;
    QLabel* mpTitle;
    QLabel* mpAuthor;
    QLabel* mpDesc;
    QLabel* mpCategory;
    QLabel* mpYear;

    OwnLineEdit* mpTitleEdit;
    OwnLineEdit* mpAuthorEdit;
    OwnLineEdit* mpDescEdit;
    QComboBox* mpCategoryEdit;
    OwnLineEdit* mpYearEdit;

    QLabel* mpCrop;
    QPushButton* mpStartCapBtn;
    QPushButton* mpUploadImgBtn;
    QPushButton* mpApplyBtn;

    OwnImageViewer* mpViewer;
    OwnScreenCapture* mpCaptrue;
    QDialog* mpParent;
};

}

#endif // OWN_ITEMUPLOADFORM_H
