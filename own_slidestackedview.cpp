#include "own_slidestackedview.h"
#include "own_element.h"
#include "own_database.h"
#include "own_database_item.h"
#include "own_util.h"

#include <QPropertyAnimation>
#include <QPainter>
#include <QByteArray>
#include <QAction>

namespace mf {

OwnSlideStackedView::OwnSlideStackedView(int category, QWidget* parent) : QStackedWidget (parent),
  mpAnime(new QPropertyAnimation(this, QByteArray())), mDuration(500), mWidgetCnt(0), mNextIdx(0), mbIsLeft(true),
  mbIsAnimation(false), mCategory(category), mpParent(parent)
{
    initRightMenu();

    connect(mpAnime, &QPropertyAnimation::valueChanged, this, &OwnSlideStackedView::valueChangedAnimation);
    connect(mpAnime, &QPropertyAnimation::finished, this, &OwnSlideStackedView::animationFininshed);

    auto pDialog = OwnConfig::getInstance()->getItemViewer();
    connect(pDialog,
            static_cast<void(OwnItemUploadView::*)(int, int)>(&OwnItemUploadView::sendItemChangedMsg),
            this,
            static_cast<void(OwnSlideStackedView::*)(int, int)>(&OwnSlideStackedView::updateElements));
}

void OwnSlideStackedView::valueChangedAnimation(QVariant value)
{
    this->mCurrentVal = value;
    this->update();
}

void OwnSlideStackedView::animationFininshed()
{
    mbIsAnimation = false;
    this->widget(this->currentIndex())->show();
    this->setCurrentIndex(mNextIdx);
}

void OwnSlideStackedView::nextWidget(int nxtIdx)
{
    if(mbIsAnimation)
        return;
    mWidgetCnt = this->count();
    int curIdx = this->currentIndex();
    mbIsLeft = curIdx < nxtIdx;
    mNextIdx = nxtIdx;
    startAnimation(curIdx);
}

void OwnSlideStackedView::startAnimation(int index)
{
    mbIsAnimation = true;
    this->widget(index)->hide();
    QRect rect = this->geometry();
    mpAnime->setStartValue(rect.width());
    mpAnime->setEndValue(rect.x());
    mpAnime->setDuration(mDuration);
    mpAnime->setEasingCurve(QEasingCurve::InOutQuad);
    mpAnime->start();
}


void OwnSlideStackedView::paintEvent(QPaintEvent *event)
{
    if(!mbIsAnimation)
        return;
    QPainter painter(this);
    paintNext(painter, mNextIdx);
    paintPrevious(painter, this->currentIndex());
}

void OwnSlideStackedView::paintPrevious(QPainter &painter, int index)
{
    QWidget *widget = this->widget(index);
    QPixmap pixmap((widget->size()));
    widget->render(&pixmap);
    QRect rect = widget->geometry();

    double value = mCurrentVal.toDouble();
    // 左上角、宽和高
    QRectF r1(0.0, 0.0, value, rect.height());
    QRectF r2(rect.width() - value, 0, value, rect.height());
    if(mbIsLeft)
    {
        painter.drawPixmap(r1, pixmap, r2);
    }
    else
    {
        painter.drawPixmap(r2, pixmap, r1);
    }
}

void OwnSlideStackedView::paintNext(QPainter &painter, int index)
{
    QWidget *widget = this->widget(index);
    QPixmap pixmap((widget->size()));
    widget->render(&pixmap);
    QRect rect = widget->geometry();

    double value = mCurrentVal.toDouble();

    QRectF r1(value, 0, rect.width() - value, rect.height());
    QRectF r2(0, 0, rect.width() - value, rect.height());
    if(mbIsLeft)
    {
        painter.drawPixmap(r1, pixmap, r2);
    }
    else
    {
        painter.drawPixmap(r2, pixmap, r1);
    }
}

void OwnSlideStackedView::updateElements(int id, int oper)
{
    // 获取新插入的数据
    odbitem item;
    item.id = id;
    item = OwnDatabase::getInstance()->select(item);
    if(item.category.value() != this->mCategory) // 如果不是一个类别的直接返回
    {
        return;
    }
    // 更新页码的变化
    auto pages = OwnUtil::getPages(this->mCategory);
    qobject_cast<OwnFadeStackedView*>(this->mpParent)->getPageBar()->setMaxPage(pages);
    // 处理页面的变化
    OwnUtil::updatePages(this, item, this->mCategory, id, static_cast<SQL_ITEM_OPER>(oper));
}

void OwnSlideStackedView::initRightMenu()
{
    auto pDialog = OwnConfig::getInstance()->getItemViewer();
    {
        auto action = new QAction("add", this);
        connect(action, static_cast<void(QAction::*)(bool)>(&QAction::triggered),
                this, [=](){ pDialog->clear(); pDialog->open(); });
        this->addAction(action);
    }
    this->setContextMenuPolicy(Qt::ActionsContextMenu);
}

}
