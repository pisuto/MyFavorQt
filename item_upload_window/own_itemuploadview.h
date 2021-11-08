#ifndef OWN_ITEMUPLOADVIEW_H
#define OWN_ITEMUPLOADVIEW_H

#include <QWidget>

#include "own_button.h"
#include "own_fadestackedview.h"
#include "own_itemuploadform.h"

namespace Ui {
class OwnItemUploadView;
}

namespace mf {

class OwnItemUploadView : public QWidget
{
    Q_OBJECT

public:
    explicit OwnItemUploadView(QWidget *parent = nullptr);
    ~OwnItemUploadView();

private slots:
    void onBtnScrClicked();
    void onBtnDocClicked();

private:
    QFrame *createForm(bool isScreenCaptrue);

private:
    Ui::OwnItemUploadView *ui;

    OwnButtonGroup* mpBtnGrp;
    OwnFadeStackedView* mpStackedView;

    QVBoxLayout* mpMainLayout;
};

}

#endif // OWN_ITEMUPLOADVIEW_H
