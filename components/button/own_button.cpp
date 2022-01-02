#include "own_button.h"
#include "../global/own_util.h"

#include <QLayout>
#include <QPropertyAnimation>
#include <QPainter>
#include <QDebug>
#include <QtMath>
#include <QAction>
#include <QMenu>
#include <QColorDialog>

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
    if(type == BTN_GRP_LAYOUT::HORIZONTAL) { /* 暂时如此规避 */
        this->setStyleSheet("background-color:rgb(245, 245, 245);");
    }
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

OwnTopButtonGroup::OwnTopButtonGroup(QWidget* parent) : QWidget(parent),
    mpParent(parent),
    mpCloseBtn(new QPushButton()),
    mpMinBtn(new QPushButton()),
    mpLayout(new QHBoxLayout())
{
    auto closeStyle = QString("QPushButton{border-image: url(:/images/windows/close.png);}"
                              "QPushButton:hover{border-image: url(:/images/windows/close-pressed.png);}"
                              "QPushButton:pressed{border-image: url(:/images/windows/close-pressed.png);}");

    auto minStyle = QString("QPushButton{border-image: url(:/images/windows/min.png);border:none;}"
                            "QPushButton:hover{border-image: url(:/images/windows/min-pressed.png);}"
                            "QPushButton:pressed{border-image: url(:/images/windows/min-pressed.png);}");
    mpCloseBtn->setStyleSheet(closeStyle);
    mpMinBtn->setStyleSheet(minStyle);

    mpCloseBtn->setFixedSize(QSize(14, 14));
    mpMinBtn->setFixedSize(QSize(14, 14));

    mpLayout->addStretch(1);
    mpLayout->addWidget(mpMinBtn);
    mpLayout->addWidget(mpCloseBtn);
    mpLayout->setContentsMargins(0,0,0,0);
    this->setLayout(mpLayout);

    connect(mpCloseBtn, &QPushButton::pressed, this, [=](){ this->checkTrayed(); });
    connect(mpMinBtn,   &QPushButton::pressed, this, [&](){ mpParent->showMinimized(); });
}


void OwnTopButtonGroup::checkTrayed()
{
    auto pConfig = OwnConfig::getInstance();
    auto& data = pConfig->getSettingData();
    if(data.trayed)
    {
        pConfig->handler(static_cast<MainWindow*>(Q_NULLPTR), HANDLER_OPER::OPER_HIDE);
        pConfig->handler(static_cast<OwnToggleButton*>(Q_NULLPTR), HANDLER_OPER::OPER_MOD, true);
        emit closeWindowToTray(true);
        return;
    }
    if(!data.modified)
    {
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(mpParent, "Trayed", "Please confirm whether to minimize?", QMessageBox::Yes | QMessageBox::No);
        if(reply == QMessageBox::Yes)
        {
            data.trayed = true;
            pConfig->handler(static_cast<OwnToggleButton*>(Q_NULLPTR), HANDLER_OPER::OPER_MOD, true);
            emit closeWindowToTray(true);
            pConfig->handler(static_cast<MainWindow*>(Q_NULLPTR), HANDLER_OPER::OPER_HIDE);
        }
        else
        {
            pConfig->handler(static_cast<OwnToggleButton*>(Q_NULLPTR), HANDLER_OPER::OPER_MOD, false);
            data.trayed = false;
            QApplication::quit();
        }
        return;
    }
    if(!data.trayed)
    {
        pConfig->handler(static_cast<OwnToggleButton*>(Q_NULLPTR), HANDLER_OPER::OPER_MOD, false);
        QApplication::quit();
    }
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


OwnToggleButton::OwnToggleButton(int trackRadius, int thumbRadius, QWidget* parent)
    : QAbstractButton(parent)
{
    setCheckable(true);
    setSizePolicy(QSizePolicy(QSizePolicy::Policy::Fixed, QSizePolicy::Policy::Fixed));
    mTrackRadius = trackRadius;
    mThumbRadius = thumbRadius;
    mAnimation = new QPropertyAnimation(this);
    mAnimation->setTargetObject(this);

    mMargin = 0 > (mThumbRadius - mTrackRadius) ? 0 : (mThumbRadius - mTrackRadius);
    mBaseOffset = mThumbRadius > mTrackRadius ? mThumbRadius : mTrackRadius;
    mEndOffset.insert(true, 4 * mTrackRadius + 2 * mMargin - mBaseOffset); // width - offset
    mEndOffset.insert(false, mBaseOffset);
    mOffset = mBaseOffset;
    QPalette palette = this->palette();

    if (mThumbRadius > mTrackRadius)
    {
        mTrackColor.insert(true, palette.highlight());
        mTrackColor.insert(false, palette.dark());
        mThumbColor.insert(true, palette.highlight());
        mThumbColor.insert(false, palette.light());
        mOpacity = 0.5f;
    }
    else
    {
        mTrackColor.insert(true, palette.highlight());
        mTrackColor.insert(false, palette.dark());
        mThumbColor.insert(true, palette.highlightedText());
        mThumbColor.insert(false, palette.light());
        mOpacity = 1.0;
    }
}

QSize OwnToggleButton::sizeHint() const
{
    int w = 4 * mTrackRadius + 2 * mMargin;
    int h = 2 * mTrackRadius + 2 * mMargin;

    return QSize(w, h);
}

void OwnToggleButton::paintEvent(QPaintEvent *)
{
    QPainter p(this);
    QPainter::RenderHints m_paintFlags = QPainter::RenderHints(QPainter::Antialiasing |
        QPainter::TextAntialiasing);
    p.setRenderHints(m_paintFlags, true);
    p.setPen(Qt::NoPen);
    bool check = isChecked();
    qreal trackOpacity = static_cast<qreal>(mOpacity);
    qreal thumbOpacity = 1.0;
    QBrush trackBrush;
    QBrush thumbBrush;

    if (this->isEnabled())
    {

        trackBrush = mTrackColor[check];
        thumbBrush = mThumbColor[check];
    }
    else
    {
        trackOpacity *= 0.8;
        trackBrush = this->palette().shadow();
        thumbBrush = this->palette().mid();
    }

    p.setBrush(trackBrush);
    p.setOpacity(trackOpacity);
    p.drawRoundedRect(mMargin, mMargin, width() - 2 * mMargin, height() - 2 * mMargin, mTrackRadius, mTrackRadius);

    p.setBrush(thumbBrush);
    p.setOpacity(thumbOpacity);
    p.drawEllipse(mOffset - mThumbRadius, mBaseOffset - mThumbRadius, 2 * mThumbRadius, 2 * mThumbRadius);
}

void OwnToggleButton::resizeEvent(QResizeEvent* e)
{
    QAbstractButton::resizeEvent(e);
    mOffset = static_cast<int>(mEndOffset.value(isChecked()));
}

void OwnToggleButton::mouseReleaseEvent(QMouseEvent  *e)
{
    QAbstractButton::mouseReleaseEvent(e);
    if (e->button() == Qt::LeftButton)
    {
        emit toggle(isChecked()); /* 发送消息 */
        mAnimation->setDuration(120);
        mAnimation->setPropertyName("mOffset");
        mAnimation->setStartValue(mOffset);
        mAnimation->setEndValue(mEndOffset[isChecked()]);
        mAnimation->start();
    }
}

void OwnToggleButton::enterEvent(QEvent * event)
{
    setCursor(Qt::PointingHandCursor);
    QAbstractButton::enterEvent(event);
}

void OwnToggleButton::setChecked(bool checked)
{
    QAbstractButton::setChecked(checked);
    mOffset = static_cast<int>(mEndOffset.value(checked));
}

int OwnToggleButton::offset()
{
    return mOffset;
}

void OwnToggleButton::setOffset(int value)
{
    mOffset = value;
    update();
}


const QColor colors[6][8] =
{
    {QColor(0, 0, 0, 255), QColor(170, 0, 0, 255), QColor(0, 85, 0, 255), QColor(170, 85, 0, 255),
    QColor(0, 170, 0, 255), QColor(170, 170, 0, 255), QColor(0, 255, 0, 255), QColor(170, 250, 0, 255)},

    {QColor(0, 0, 127, 255), QColor(170, 0, 127, 255), QColor(0, 85, 127, 255), QColor(170, 85, 127, 255),
    QColor(0, 170, 127, 255), QColor(170, 170, 127, 255), QColor(0, 255, 127, 255), QColor(170, 255, 127, 255)},

    {QColor(0, 0, 255, 255), QColor(170, 0, 255, 255), QColor(0, 85, 255, 255), QColor(170, 85, 255, 255),
    QColor(0, 170, 255, 255), QColor(170, 170, 255, 255), QColor(0, 255, 255, 255), QColor(170, 255, 255, 255)},

    {QColor(85, 0, 0, 255), QColor(255, 0, 0, 255), QColor(85, 85, 0, 255), QColor(255, 85, 0, 255),
    QColor(85, 170, 0, 255), QColor(255, 170, 0, 255), QColor(85, 255, 0, 255), QColor(255, 255, 0, 255)},

    {QColor(85, 0, 127, 255), QColor(255, 0, 127, 255), QColor(85, 85, 127, 255), QColor(255, 85, 127, 255),
    QColor(85, 170, 127, 255), QColor(255, 170, 127, 255), QColor(85, 255, 127, 255), QColor(255, 255, 127, 255)},

    {QColor(85, 0, 255, 255), QColor(255, 0, 255, 255), QColor(85, 85, 255, 255), QColor(255, 85, 255, 255),
    QColor(85, 170, 255, 255), QColor(255, 170, 255, 255), QColor(85, 255, 255, 255), QColor(255, 255, 255, 255)}
};

OwnColorCombox::OwnColorCombox(QWidget *parent)
    : QPushButton(parent), mIconSize(56, 18)
{
    this->setMenu(createColorMenu(SLOT(onColorChanged()), SLOT(onShowColorBoard())));
    this->setAutoFillBackground(true);
    this->setButtonIcon(QColor(255, 255, 255));

    auto btnStyle = QString("QPushButton::menu-indicator{image:url(:/images/windows/down-arrow.png);"
                            "width:18px;height:18px;subcontrol-position:right center;position: absolute;right:2px;}"
                            "QPushButton{font-family:'Microsoft YaHei';font-size:12px;color:rgb(150,150,150);}"
                            "QPushButton{background-color:rgb(245, 245, 245);border:2px solid #dcdfe6;"
                            "border-radius:5px;padding-right:18%;}");
    this->setStyleSheet(btnStyle);
    this->setFixedSize(QSize(90, 30));
    this->setIconSize(mIconSize);

    auto menuStyle = QString("QMenu{font-size:12px;color:rgb(150,150,150);background-color:white;border-radius:5px;"
                             "border:2px solid #dcdfe6;;padding:5px 4px 5px 4px;margin:1px;}");
    this->menu()->setStyleSheet(menuStyle);
    this->menu()->setAttribute(Qt::WA_TranslucentBackground); //Menu背景透明
    this->menu()->setWindowFlags(this->menu()->windowFlags()  | Qt::FramelessWindowHint | Qt::NoDropShadowWindowHint);
}

void OwnColorCombox::setButtonIcon(QColor color)
{
    this->setIcon(createColorIcon(color));

}

QMenu *OwnColorCombox::createColorMenu(const char *slot, const char *slotColorBoard)
{
    // 选择其他颜色
    QToolButton *pBtnOtherColor = new QToolButton;
    pBtnOtherColor->setText(tr("other color"));
    pBtnOtherColor->setFixedSize(QSize(142, 20));
    pBtnOtherColor->setAutoRaise(true);
    pBtnOtherColor->setToolTip("other color");
    connect(pBtnOtherColor, SIGNAL(clicked()), this, slotColorBoard);

    // 基本色
    QGridLayout *pGridLayout = new QGridLayout;
    pGridLayout->setAlignment(Qt::AlignCenter);
    pGridLayout->setContentsMargins(0, 0, 0, 0);
    pGridLayout->setSpacing(2);

    for (int iRow = 0; iRow < 6; iRow++)
    {
        for (int iCol = 0; iCol < 8; iCol++)
        {
            QAction *action = new QAction(this);
            action->setData(colors[iRow][iCol]);
            action->setIcon(createColorIcon(colors[iRow][iCol]));
            connect(action, SIGNAL(triggered()), this, slot);

            QToolButton *pBtnColor = new QToolButton;
            pBtnColor->setFixedSize(QSize(16, 16));
            pBtnColor->setAutoRaise(true);
            pBtnColor->setDefaultAction(action);
            pBtnColor->setToolTip("white");

            pGridLayout->addWidget(pBtnColor, iRow, iCol);
        }
    }

    QWidget *widget = new QWidget;
    widget->setLayout(pGridLayout);

    QVBoxLayout *pVLayout = new QVBoxLayout;
    // pVLayout->addWidget(pBtnTransparent);
    pVLayout->addWidget(widget);
    pVLayout->addWidget(pBtnOtherColor);

    QMenu *colorMenu = new QMenu(this);
    colorMenu->setLayout(pVLayout);

    return colorMenu;
}

QIcon OwnColorCombox::createColorIcon(QColor color)
{
    QPixmap pixmap(mIconSize.width(), mIconSize.height());
    QPainter painter(&pixmap);
    painter.setPen(Qt::NoPen);
    painter.fillRect(QRect(0, 0, mIconSize.width(), mIconSize.height()), color);

    return QIcon(pixmap);
}

void OwnColorCombox::onColorChanged()
{
    QAction *pFillColorAction = new QAction(this);
    pFillColorAction = qobject_cast<QAction *>(sender());
    QColor color = qvariant_cast<QColor>(pFillColorAction->data());

    this->menu()->close();
    setButtonIcon(color);
    emit colorChanged(color);
}

void OwnColorCombox::onShowColorBoard()
{
    this->menu()->close();
    QColor color = QColorDialog::getColor(Qt::black, this);
    if (!color.isValid())
        return;
    setButtonIcon(color);
    emit colorChanged(color);
}


}
