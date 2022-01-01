#ifndef OWN_ADDLABELVIEW_H
#define OWN_ADDLABELVIEW_H

#include <QDialog>
#include <QLayout>

class QFrame;

namespace Ui {
class OwnAddLabelView;
}

namespace mf {

class OwnAddLabelView : public QDialog
{
    Q_OBJECT
public:
    explicit OwnAddLabelView(QWidget *parent = nullptr);
    ~OwnAddLabelView();

private:
    QFrame* createDropImageBox();

    Ui::OwnAddLabelView *ui;

    QVBoxLayout* mpLayout;
};

}

#endif // OWN_ADDLABELVIEW_H
