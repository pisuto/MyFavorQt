#include "own_button.h"
#include "own_util.h"

#include <QLayout>
#include <QPropertyAnimation>
#include <QPainter>
#include <QDebug>
#include <QtMath>

namespace mf {

OwnButtonGroup::OwnButtonGroup(QWidget* parent) : QWidget(parent),
    mpAnime(new QPropertyAnimation(this, QByteArray())), mpLayout(new QHBoxLayout),
    mCurIdx(0), mPreIdx(0), mBtnSize(QSize()), mLineHeight(3),
    mLineColor(QColor(20, 20, 20))
{
    mpAnime->setDuration(300);

    mpLayout->setContentsMargins(0, 0, 0, 0);
    mpLayout->setSpacing(0);
    this->setLayout(mpLayout);
    this->setAttribute(Qt::WidgetAttribute::WA_StyledBackground);  // 重要
    connect(mpAnime, &QPropertyAnimation::valueChanged, this, &OwnButtonGroup::valueChangedAnimation);
}

void OwnButtonGroup::addButton(OwnButton *pBtn)
{
    mBtnGrp.push_back(pBtn);
    mpLayout->addWidget(pBtn);
}

void OwnButtonGroup::valueChangedAnimation(QVariant value)
{
    mCurrVal = value;
    this->update();
}

QRectF OwnButtonGroup::getCurrLength(double value)
{
    auto length = mBtnGrp[mCurIdx]->pos().x() - mBtnGrp[mPreIdx]->pos().x();
    auto offset = value - mBtnGrp[mPreIdx]->pos().x();
    auto ratio = OwnUtil::triangleFunc(length == 0 ? 0.f : offset / length);
    auto base = qAbs(length / mBtnSize.width());

    if(length < 0)
    {
        return QRectF(value,
                      this->height() - mLineHeight,
                      (1 + base * ratio) * mBtnSize.width(),
                      mLineHeight);
    }
    else if(length > 0)
    {
        return QRectF(value + mBtnSize.width(),
                      this->height() - mLineHeight,
                      -(1 + base * ratio) * mBtnSize.width(),
                      mLineHeight);
    }
    return QRectF(value,
                  this->height() - mLineHeight,
                  mBtnSize.width(),
                  mLineHeight);
}

void OwnButtonGroup::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(Qt::NoPen);
    painter.setBrush(mLineColor);
    painter.drawRect(getCurrLength(mCurrVal.toDouble()));
}

void OwnButtonGroup::onButtonClicked(int index)
{
    mBtnGrp[index]->setChecked(true);
    mPreIdx = mCurIdx;
    mCurIdx = index;
    // top
    mpAnime->setStartValue(mBtnGrp[mPreIdx]->pos().x());
    mpAnime->setEndValue(mBtnGrp[mCurIdx]->pos().x());
    mpAnime->setEasingCurve(QEasingCurve::InCubic);
    mpAnime->start();
}

void OwnButtonGroup::initButtonConnect()
{
    for(int i = 0; i < mBtnGrp.size(); ++i)
    {
        connect(mBtnGrp[i], &OwnButton::sendIndex, this, &OwnButtonGroup::onButtonClicked);
    }
}

void OwnButtonGroup::setNormalButtonSize(QSize btnSize)
{
    mBtnSize = btnSize;
}

void OwnTopButtonGroup::checkTrayed()
{
    auto pConfig = OwnConfig::getInstance();
    if(pConfig->getTrayed())
    {
        pConfig->hideWindowToTray();
    }
    else
    {
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(mpParent, "Trayed", "Need to minmize to tray or just quit?", QMessageBox::Yes | QMessageBox::No);
        if(reply == QMessageBox::Yes)
        {
            pConfig->setTrayed(true);
            pConfig->hideWindowToTray();

        }
        else
        {
            pConfig->setTrayed(false);
            QApplication::quit();
            return; // 不是立即退出
        }
    }

    emit closeWindowToTray(true);
}

}
