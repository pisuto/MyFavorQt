#include "item_upload_window/own_itemuploadview.h"
#include "ui_OwnItemUploadView.h"

namespace mf {

OwnItemUploadView::OwnItemUploadView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::OwnItemUploadView)
{
    ui->setupUi(this);
}

OwnItemUploadView::~OwnItemUploadView()
{
    delete ui;
}

}
