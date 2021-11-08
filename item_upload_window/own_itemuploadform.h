#ifndef OWN_ITEMUPLOADFORM_H
#define OWN_ITEMUPLOADFORM_H

#include <QFrame>

#include "item_upload_window/own_screencapture.h"

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
    explicit OwnItemUploadForm(bool screenMethod = false, QWidget* parent = nullptr);

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

    QLabel* mpImage;
    QPushButton* mpStartCapBtn;

    bool mbImgMethod; /* 默认false是截图 */

    OwnScreenCapture* mpCaptrue;

};

}

#endif // OWN_ITEMUPLOADFORM_H
