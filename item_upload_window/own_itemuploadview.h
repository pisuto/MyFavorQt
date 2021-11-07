#ifndef OWN_ITEMUPLOADVIEW_H
#define OWN_ITEMUPLOADVIEW_H

#include <QWidget>

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

private:
    Ui::OwnItemUploadView *ui;
};

}

#endif // OWN_ITEMUPLOADVIEW_H
