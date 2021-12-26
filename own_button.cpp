#include "own_button.h"
#include "own_util.h"

#include <QLayout>
#include <QPropertyAnimation>
#include <QPainter>
#include <QDebug>
#include <QtMath>

namespace mf {

OwnButtonGroup::OwnButtonGroup(QWidget* parent, BTN_GRP_LAYOUT type) : QWidget(parent),
    mpAnime(new QPropertyAnimation(this, QByteArray())),
    mCurIdx(0), mPreIdx(0), mCurrVal(0), mBtnSize(QSize()),
    mLineWeight(3), mLineColor(QColor(20, 20, 20)), mType(type)
{
    if(mType == BTN_GRP_LAYOUT::HORIZONTAL) {
        mpLayout = new QHBoxLayout;
    }
    else {
        mpLayout = new QVBoxLayout;
    }
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
    if(mType == BTN_GRP_LAYOUT::HORIZONTAL)
    {
        auto length = mBtnGrp[mCurIdx]->pos().x() - mBtnGrp[mPreIdx]->pos().x();
        auto offset = value - mBtnGrp[mPreIdx]->pos().x();
        auto ratio = OwnUtil::triangleFunc(length == 0 ? 0.f : offset / length);
        auto base = qAbs(length / mBtnSize.width());

        if(length < 0)
        {
            return QRectF(value,
                          mBtnSize.height() - mLineWeight,
                          (1 + base * ratio) * mBtnSize.width(),
                          mLineWeight);
        }
        else if(length > 0)
        {
            return QRectF(value + mBtnSize.width(),
                          mBtnSize.height() - mLineWeight,
                          -(1 + base * ratio) * mBtnSize.width(),
                          mLineWeight);
        }
        return QRectF(value,
                      mBtnSize.height() - mLineWeight,
                      mBtnSize.width(),
                      mLineWeight);
    }
    else /* 竖直情况 */
    {
        auto length = mBtnGrp[mCurIdx]->pos().y() - mBtnGrp[mPreIdx]->pos().y();
        auto offset = value - mBtnGrp[mPreIdx]->pos().y();
        auto ratio = OwnUtil::triangleFunc(length == 0 ? 0.f : offset / length);
        auto base = qAbs(length / mBtnSize.height());

        if(length < 0)
        {
            return QRectF(mBtnSize.width() - mLineWeight,
                          value,
                          mLineWeight,
                          (1 + base * ratio) * mBtnSize.height());
        }
        else if(length > 0)
        {
            return QRectF(mBtnSize.width() - mLineWeight,
                          value + mBtnSize.height(),
                          mLineWeight,
                          -(1 + base * ratio) * mBtnSize.height());
        }
        return QRectF(mBtnSize.width() - mLineWeight,
                      value,
                      mLineWeight,
                      mBtnSize.height());
    }
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
    mpAnime->setStartValue(mType == BTN_GRP_LAYOUT::HORIZONTAL? mBtnGrp[mPreIdx]->pos().x() : mBtnGrp[mPreIdx]->pos().y());
    mpAnime->setEndValue(mType == BTN_GRP_LAYOUT::HORIZONTAL ? mBtnGrp[mCurIdx]->pos().x() : mBtnGrp[mCurIdx]->pos().y());
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

void OwnButtonGroup::setNormalButtonSize(QSize btnSize) /* 初始化时调用 */
{
    mBtnSize = btnSize;
    if(mType == BTN_GRP_LAYOUT::VERTICAL) {
        this->setFixedWidth(btnSize.width()); /* 不进行限制会自适应父级的宽度 */
    }
    else {
        this->setFixedHeight(btnSize.height());
    }
}

void OwnTopButtonGroup::checkTrayed()
{
    auto pConfig = OwnConfig::getInstance();
    auto& data = pConfig->getSettingData();
    if(data.trayed)
    {
        pConfig->hideWindowToTray();
    }
    else
    {
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(mpParent, "Trayed", "Need to minmize to tray or just quit?", QMessageBox::Yes | QMessageBox::No);
        if(reply == QMessageBox::Yes)
        {
            data.trayed = true;
            pConfig->hideWindowToTray();

        }
        else
        {
            data.trayed = false;
            QApplication::quit();
            return; // 不是立即退出
        }
    }

    emit closeWindowToTray(true);
}

void OwnButton::setButtonStyle(int font, QRgb normal, QRgb hover, QRgb pressed, QString family)
{
    auto style = QString("QPushButton{background-color:%3;border:none;font-size:%1;font-family:'%2';}"
                         "QPushButton:hover{background-color:%4;}"
                         "QPushButton:pressed{background-color:%5;}")
            .arg(font).arg(family).arg(OwnUtil::QRgbaToQString(normal))
            .arg(OwnUtil::QRgbaToQString(hover)).arg(OwnUtil::QRgbaToQString(pressed));
    this->setStyleSheet(style);
    connect(this, &QPushButton::clicked, this, &OwnButton::ownButtonClicked);
}

}
