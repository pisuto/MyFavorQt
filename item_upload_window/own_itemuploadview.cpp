#include "item_upload_window/own_itemuploadview.h"
#include "ui_OwnItemUploadView.h"

namespace mf {

OwnItemUploadView::OwnItemUploadView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::OwnItemUploadView),
    mpBtnGrp(new OwnButtonGroup(0)),
    mpStackedView(new OwnFadeStackedView(Q_NULLPTR, this)),
    mpMainLayout(new QVBoxLayout)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::FramelessWindowHint); //无边框

    auto screenCaptureBtn =
            new OwnButton(0, QIcon(":/images/svgtopng/anime.png"), "Screen");
    auto imageUploadBtn =
            new OwnButton(1, QIcon(":/images/svgtopng/photos.png"), "Document");
    connect(screenCaptureBtn, &QPushButton::clicked, this, &OwnItemUploadView::onBtnScrClicked);
    connect(imageUploadBtn, &QPushButton::clicked, this, &OwnItemUploadView::onBtnDocClicked);
    mpBtnGrp->addButton(screenCaptureBtn);
    mpBtnGrp->addButton(imageUploadBtn);
    mpBtnGrp->addStretch(1);
    mpBtnGrp->initButtonConnect();

    // 初始化选择界面
    mpStackedView->addWidget(createForm(false));
    mpStackedView->addWidget(createForm(true));

    // 设置末尾的保存和取消按钮
    auto pSaveBtn = new QPushButton();
    auto pCancelBtn = new QPushButton();
    pSaveBtn->setText("Save");
    pCancelBtn->setText("Cancel");
    connect(pSaveBtn, &QPushButton::clicked, this, [&](){ this->close(); });
    connect(pCancelBtn, &QPushButton::clicked, this, [&](){ this->close(); });
    auto pHorLayout = new QHBoxLayout();
    pHorLayout->addWidget(pSaveBtn);
    pHorLayout->addWidget(pCancelBtn);

    // 设置边框
    mpMainLayout->setContentsMargins(5, 5, 5, 5);
    mpMainLayout->addWidget(mpBtnGrp);
    mpMainLayout->addWidget(mpStackedView);
    mpMainLayout->addLayout(pHorLayout);
    this->setLayout(mpMainLayout);
}



OwnItemUploadView::~OwnItemUploadView()
{
    delete ui;
}

void OwnItemUploadView::onBtnScrClicked()
{
    mpStackedView->switchWidget(0);
}

void OwnItemUploadView::onBtnDocClicked()
{
    mpStackedView->switchWidget(1);
}

QFrame *OwnItemUploadView::createForm(bool isScreenCaptrue)
{
    return new OwnItemUploadForm(isScreenCaptrue);
}

}
