#include "item_upload_window/own_itemuploadview.h"
#include "ui_OwnItemUploadView.h"
#include "own_database.h"
#include "own_database_item.h"
#include "own_util.h"

#include <QPainter>
#include <QComboBox>
#include <QLineEdit>

namespace mf {

OwnItemUploadView::OwnItemUploadView(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::OwnItemUploadView),
    mpBtnGrp(new OwnButtonGroup(0)),
    mpStackedView(new OwnFadeStackedView(Q_NULLPTR, this)),
    mpMainLayout(new QVBoxLayout),
    mDragPos(QPoint(0, 0)), mIdentification(-1)
{
    ui->setupUi(this);

    this->setAttribute(Qt::WA_TranslucentBackground);
    this->setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint);

    auto screenCaptureBtn =
            new OwnButton(0, QIcon(":/images/svgtopng/anime.png"), "New Item");
    connect(screenCaptureBtn, &QPushButton::clicked, this, [&](){ mpStackedView->switchWidget(0); });
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
    // 插入该模板
    connect(pSaveBtn, &QPushButton::clicked, this,
            [&](){ this->updateItem(this->getId() == -1 ? SQL_ITEM_OPER::INSERT : SQL_ITEM_OPER::UPDATE); });
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

        auto imgHeight = OwnConfig::getInstance()->getDisplayImageSize().height();
        if(mDragPos.y() > imgHeight / 10)
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

void OwnItemUploadView::updateItem(SQL_ITEM_OPER oper)
{
    auto pInfo = qobject_cast<OwnItemUploadForm*>(mpStackedView->currentWidget());
    if(!pInfo) return;

    auto pTitle = qobject_cast<OwnLineEdit*>
            (pInfo->getFormInfoByTag(SQL_TABLE_ITEM::ITEM_TITLE));
    if(!pTitle) return;
    auto pAuthor = qobject_cast<OwnLineEdit*>
            (pInfo->getFormInfoByTag(SQL_TABLE_ITEM::ITEM_AUTHOR));
    if(!pAuthor) return;
    auto pDesc = qobject_cast<OwnLineEdit*>
            (pInfo->getFormInfoByTag(SQL_TABLE_ITEM::ITEM_DESC));
    if(!pDesc) return;
    auto pViewer = qobject_cast<OwnImageViewer*>
            (pInfo->getFormInfoByTag(SQL_TABLE_ITEM::ITEM_IMAGEPATH));
    if(!pViewer) return;
    auto pCategory = qobject_cast<QComboBox*>
            (pInfo->getFormInfoByTag(SQL_TABLE_ITEM::ITEM_CATEGORY));
    if(!pCategory) return;
    auto pYear = qobject_cast<OwnLineEdit*>
            (pInfo->getFormInfoByTag(SQL_TABLE_ITEM::ITEM_CRATEYEAR));
    if(!pYear) return;

    auto pDataBase = OwnDatabase::getInstance();
    // 插入数据
    odbitem item;
    int id = this->getId();
    item.title = pTitle->text();
    item.author = pAuthor->text();
    item.desc = pDesc->text();
    item.category = pCategory->currentIndex() + 1;
    item.create_year = pYear->text().toInt();
    if(oper == SQL_ITEM_OPER::INSERT) {
        item.label = 0;
        id = pDataBase->insert(item);
    }

    // 更新数据
    auto imgName = QString::number(id) + ".png";
    pViewer->getFinalPixmap().save(SQL_TABLE_ITEM::ImgFileLocation() + "/" + imgName); // 保存图片
    item.id = id;
    item.path = imgName;
    pDataBase->update(item);

    // 更新类别数目信息
    OwnConfig::getInstance()->updateCategoryCount(item.category.value());

    // 发送数据变化信号，对整体页面进行更新
    emit sendItemChangedMsg(id, static_cast<int>(oper));

    this->close();
}

void OwnItemUploadView::clear()
{
    auto pInfo = qobject_cast<OwnItemUploadForm*>(mpStackedView->currentWidget());
    pInfo->clearData();
    mIdentification = -1;
}

void OwnItemUploadView::openThisItem(int id)
{
    auto pInfo = qobject_cast<OwnItemUploadForm*>(mpStackedView->currentWidget());
    if(!pInfo) return;

    auto pTitle = qobject_cast<OwnLineEdit*>
            (pInfo->getFormInfoByTag(SQL_TABLE_ITEM::ITEM_TITLE, SQL_ITEM_OPER::UPDATE));
    if(!pTitle) return;
    auto pAuthor = qobject_cast<OwnLineEdit*>
            (pInfo->getFormInfoByTag(SQL_TABLE_ITEM::ITEM_AUTHOR, SQL_ITEM_OPER::UPDATE));
    if(!pAuthor) return;
    auto pDesc = qobject_cast<OwnLineEdit*>
            (pInfo->getFormInfoByTag(SQL_TABLE_ITEM::ITEM_DESC, SQL_ITEM_OPER::UPDATE));
    if(!pDesc) return;
    auto pViewer = qobject_cast<OwnImageViewer*>
            (pInfo->getFormInfoByTag(SQL_TABLE_ITEM::ITEM_IMAGEPATH, SQL_ITEM_OPER::UPDATE));
    if(!pViewer) return;
    auto pCategory = qobject_cast<QComboBox*>
            (pInfo->getFormInfoByTag(SQL_TABLE_ITEM::ITEM_CATEGORY, SQL_ITEM_OPER::UPDATE));
    if(!pCategory) return;
    auto pYear = qobject_cast<OwnLineEdit*>
            (pInfo->getFormInfoByTag(SQL_TABLE_ITEM::ITEM_CRATEYEAR, SQL_ITEM_OPER::UPDATE));
    if(!pYear) return;

    auto pDataBase = OwnDatabase::getInstance();
    this->setId(id);
    // 读取数据
    odbitem item;
    item.id = this->getId();
    item = pDataBase->select(item);
    // 更新界面
    pTitle->setText(item.title.value());
    pAuthor->setText(item.author.value());
    pDesc->setText(item.desc.value());
    pCategory->setCurrentIndex(item.category.value() - 1);
    pYear->setText(QString::number(item.create_year.value()));
    // 更新截图
    pViewer->reset(SQL_TABLE_ITEM::ImgFileLocation() + "/" + item.path.value());
}

void OwnItemUploadView::delItem(int id)
{
    // 删除操作，对整体页面进行更新
    // 还是不能在这里删除数据库条目

    emit sendItemChangedMsg(id, static_cast<int>(SQL_ITEM_OPER::DELETE));

}

}
