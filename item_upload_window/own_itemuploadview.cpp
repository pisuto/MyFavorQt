#include "item_upload_window/own_itemuploadview.h"
#include "ui_OwnItemUploadView.h"

#include <QPainter>

namespace mf {

OwnItemUploadView::OwnItemUploadView(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::OwnItemUploadView),
    mpBtnGrp(new OwnButtonGroup(0)),
    mpStackedView(new OwnFadeStackedView(Q_NULLPTR, this)),
    mpMainLayout(new QVBoxLayout),
    mDragPos(QPoint(0, 0))
{
    ui->setupUi(this);

    this->setAttribute(Qt::WA_TranslucentBackground);
    this->setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint);

    auto screenCaptureBtn =
            new OwnButton(0, QIcon(":/images/svgtopng/anime.png"), "New Item");
    connect(screenCaptureBtn, &QPushButton::clicked, this, &OwnItemUploadView::onBtnScrClicked);
    mpBtnGrp->addButton(screenCaptureBtn);
    mpBtnGrp->addStretch(1);
    mpBtnGrp->initButtonConnect();

    // 初始化选择界面
    mpStackedView->addWidget(new OwnItemUploadForm(this));

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

void OwnItemUploadView::paintEvent(QPaintEvent *e)
{
    Q_UNUSED(e)
    // 绘制圆角
    QPainter p(this);
    p.setPen(Qt::NoPen);
    p.setBrush(QBrush(QColor(0, 0, 0, 0)));
    p.drawRect(rect());
    p.setRenderHint(QPainter::Antialiasing);
    p.setBrush(palette().color(QPalette::Window));
    p.drawRoundedRect(0, 0, width() - 1, height() - 1, 6, 6);
}

void OwnItemUploadView::mouseMoveEvent(QMouseEvent* event)
{
    if (event->buttons() & Qt::LeftButton)
    {
        if(mDragPos.y() > static_cast<int>(IMAGE_DISPLAY_SIZE::HEIGHT) / 10)
            return;
        auto desktopRc = QApplication::desktop()->availableGeometry();
        auto curPoint = event->globalPos() - mDragPos;
        if (event->globalY() > desktopRc.height())
        {
            curPoint.setY(desktopRc.height() - mDragPos.y());
        }
        this->move(curPoint);
    }
    QWidget::mouseMoveEvent(event);
}

void OwnItemUploadView::mousePressEvent(QMouseEvent*event)
{
    if (event->button() == Qt::LeftButton)
    {
        mDragPos = QCursor::pos() - frameGeometry().topLeft();
    }
    QWidget::mousePressEvent(event);
}

}
