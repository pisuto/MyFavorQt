#ifndef OWN_ITEMUPLOADFORM_H
#define OWN_ITEMUPLOADFORM_H

#include <QFrame>

#include "own_screencapture.h"
#include "own_imageviewer.h"

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

private:
    void chooseImageDialog();

private:
    QVBoxLayout* mpLayout;
    QLabel* mpTitle;
    QLabel* mpAuthor;
    QLabel* mpDesc;
    QLabel* mpCategory;
    QLabel* mpYear;

    QLineEdit* mpTitleEdit;
    QLineEdit* mpAuthorEdit;
    QLineEdit* mpDescEdit;
    QComboBox* mpCategoryEdit;
    QLineEdit* mpYearEdit;

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
