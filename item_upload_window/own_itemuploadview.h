#ifndef OWN_ITEMUPLOADVIEW_H
#define OWN_ITEMUPLOADVIEW_H

#include <QWidget>
#include <QDialog>

#include "own_button.h"
#include "own_fadestackedview.h"
#include "own_itemuploadform.h"

namespace Ui {
class OwnItemUploadView;
}

namespace mf {

class OwnItemUploadView : public QDialog
{
    Q_OBJECT

public:
    explicit OwnItemUploadView(QWidget *parent = nullptr);
    ~OwnItemUploadView() override ;

    void clear();
    void openThisItem(int id);
    void delItem(int id);

    void setId(int id) { mIdentification = id; }
    int getId() const { return mIdentification; }

signals:
    void sendItemChangedMsg(int, int);

protected:
    void paintEvent(QPaintEvent* e) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mousePressEvent(QMouseEvent*event) override;

private:
    void updateItem(SQL_ITEM_OPER oper);
    void clearItemData();
private:
    Ui::OwnItemUploadView *ui;

    OwnButtonGroup* mpBtnGrp;
    OwnFadeStackedView* mpStackedView;
    QVBoxLayout* mpMainLayout;
    QPoint mDragPos;
    int mIdentification;
};

}

#endif // OWN_ITEMUPLOADVIEW_H
