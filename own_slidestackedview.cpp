#include "own_slidestackedview.h"

#include <QPropertyAnimation>
#include <QPainter>
#include <QByteArray>
#include <QAction>

namespace mf {

OwnSlideStackedView::OwnSlideStackedView(QWidget* parent) : QStackedWidget (parent),
  mpAnime(new QPropertyAnimation(this, QByteArray())), mDuration(500), mbIsLeft(true),
  mbIsAnimation(false),
  mpUploadView(new OwnItemUploadView())
{
    initRightMenu();
    connect(mpAnime, &QPropertyAnimation::valueChanged, this, &OwnSlideStackedView::valueChangedAnimation);
    connect(mpAnime, &QPropertyAnimation::finished, this, &OwnSlideStackedView::animationFininshed);
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
    QWidget *nextWidget = this->widget(index);
    QRect rect = this->geometry();
    nextWidget->resize(rect.width(), rect.height());
    QPixmap nextPixmap(nextWidget->size());
    nextWidget->render(&nextPixmap);
    double value = mCurrentVal.toDouble();
    QRectF r1(value, 0, rect.width() - value, rect.height());
    QRectF r2(0, 0, rect.width() - value, rect.height());
    if(mbIsLeft)
    {
        painter.drawPixmap(r1, nextPixmap, r2);
    }
    else
    {
        painter.drawPixmap(r2, nextPixmap, r1);
    }
}

void OwnSlideStackedView::initRightMenu()
{
    {
        auto action = new QAction(this);
        action->setText("add");
        connect(action, static_cast<void(QAction::*)(bool)>(&QAction::triggered),
                this, &OwnSlideStackedView::addOperAction);
        this->addAction(action);
    }
    {
        auto action = new QAction(this);
        action->setText("delete");
        connect(action, static_cast<void(QAction::*)(bool)>(&QAction::triggered),
                this, &OwnSlideStackedView::deleteOperAction);
        this->addAction(action);
    }

    this->setContextMenuPolicy(Qt::ActionsContextMenu);
}

void OwnSlideStackedView::deleteOperAction()
{

}

void OwnSlideStackedView::addOperAction()
{
    mpUploadView->show();
}



}
