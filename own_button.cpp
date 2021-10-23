#include "own_button.h"

#include <QLayout>
#include <QPropertyAnimation>
#include <QPainter>
#include <QDebug>
#include <QtMath>

namespace mf {

OwnButtonGroup::OwnButtonGroup(QWidget* parent) : QWidget(parent),
    mpAnime(new QPropertyAnimation(this, QByteArray())),
    mpBtnGrp(new QButtonGroup(this)), mpLayout(new QHBoxLayout),
    mCurIdx(0), mPreIdx(0), mCurrVal(0), mBtnSize(QSize()), mLineHeight(3),
    mLineColor(QColor(20, 20, 20))
{
    this->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    mpAnime->setDuration(300);

    mpLayout->setContentsMargins(0, 0, 0, 0);
    mpLayout->setSpacing(0);
    this->setLayout(mpLayout);

    connect(mpBtnGrp, static_cast<void(QButtonGroup::*)(int)>(&QButtonGroup::buttonClicked),
            this, &OwnButtonGroup::onButtonClicked);
    connect(mpAnime, &QPropertyAnimation::valueChanged, this, &OwnButtonGroup::valueChangedAnimation);
}

void OwnButtonGroup::addButton(QPushButton *pBtn, int id)
{
    pBtn->setFixedSize(80,30);
    mpBtnGrp->addButton(pBtn, id);
    mpLayout->addWidget(pBtn);
    mBtnSize = pBtn->size();

    auto style = QString("QPushButton{background-color:rgba(255,255,255,40);border:none;}"
                         "QPushButton:hover{background-color:rgba(211,211,211,40);}"
                         "QPushButton:pressed{background-color:rgba(105,105,105,80);}");
    pBtn->setStyleSheet(style);
}

void OwnButtonGroup::valueChangedAnimation(QVariant value)
{
    mCurrVal = value;
    this->update();
}

int OwnButtonGroup::getCurrLength(int value)
{
    auto length = (mCurIdx - mPreIdx) * mBtnSize.width();
    auto offset = value - mPreIdx * mBtnSize.width();
    auto ratio = qSin(M_PI * offset / length);
    auto base = qAbs(mCurIdx - mPreIdx);
    auto res = (1 + (base - 1) * ratio) * mBtnSize.width();
    return base == 0 ? mBtnSize.width() : res;
}

void OwnButtonGroup::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(Qt::NoPen);
    painter.setBrush(mLineColor);
    painter.drawRoundedRect(QRect(
                                mCurrVal.toInt(),
                                this->height() - mLineHeight,
                                getCurrLength(mCurrVal.toInt()),
                                mLineHeight),
                            mLineHeight*2/3,
                            mLineHeight*2/3);
}

void OwnButtonGroup::onButtonClicked(int index)
{
    mpBtnGrp->button(index)->setChecked(true);
    mPreIdx = mCurIdx;
    mCurIdx = index;

    // top
    mpAnime->setStartValue(mPreIdx * mBtnSize.width());
    mpAnime->setEndValue(mCurIdx * mBtnSize.width());
    mpAnime->setEasingCurve(QEasingCurve::InCubic);
    mpAnime->start();
}


}
