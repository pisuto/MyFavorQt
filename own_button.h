#ifndef OWN_BUTTON_H
#define OWN_BUTTON_H

#include <QPushButton>
#include <QButtonGroup>
#include <QLayout>
#include <QVariant>
#include <QApplication>
#include <QMouseEvent>
#include <QDesktopWidget>
#include <QMessageBox>
#include <QToolButton>
#include <QComboBox>

class QPropertyAnimation;

namespace mf {

enum BTN_GRP_LAYOUT {
    VERTICAL = 0,
    HORIZONTAL = 1,
};

class OwnButton : public QPushButton
{
    Q_OBJECT
public:
    explicit OwnButton(const int& index, const QString& text, QWidget* parent = Q_NULLPTR) :
        QPushButton(text, parent), mIndex(index) {}
    OwnButton(const int& index, const QIcon& icon, const QString& text, QWidget* parent = Q_NULLPTR) :
        QPushButton(icon, text, parent), mIndex(index) {}
    ~OwnButton() {}

    int geIndex() { return mIndex; }
    void setButtonStyle(int font = 15, QRgb normal = qRgba(255, 255, 255, 40),
                        QRgb hover = qRgba(211, 211, 211, 40), QRgb pressed = qRgba(105, 105, 105, 80), QString family = "Microsoft YaHei UI");

signals:
    void sendIndex(int index);

public slots:
    void ownButtonClicked() { emit sendIndex(mIndex); }

private:
    int mIndex;
};

class OwnButtonGroup : public QWidget
{
    Q_OBJECT
public:
    explicit OwnButtonGroup(QWidget* parent = nullptr, BTN_GRP_LAYOUT type = BTN_GRP_LAYOUT::HORIZONTAL);
    ~OwnButtonGroup() {}

    void initButtonConnect();
    void setNormalButtonSize(QSize btnSize);
    void addButton(OwnButton* pBtn);
    void addStretch(int flag) { mpLayout->addStretch(flag); }
    void setOffset(size_t val) { mCurrVal = val; }
public slots:
    void valueChangedAnimation(QVariant value);
    void onButtonClicked(int index);

protected:
    void paintEvent(QPaintEvent *event);

private:
    QRectF getCurrLength(double value);

private:
    QPropertyAnimation* mpAnime;
    QVector<OwnButton*> mBtnGrp;
    QBoxLayout* mpLayout;

    int mCurIdx;
    int mPreIdx;
    QVariant mCurrVal;
    QSize mBtnSize;
    int mLineWeight;
    QColor mLineColor;
    BTN_GRP_LAYOUT mType;
};

class OwnTopButtonGroup : public QWidget
{
    Q_OBJECT
public:
    explicit OwnTopButtonGroup(QWidget* parent = nullptr) : QWidget(parent),
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

    ~OwnTopButtonGroup() {}

signals:
    void closeWindowToTray(bool);

private:
    void checkTrayed();

    QWidget* mpParent;
    QPushButton* mpCloseBtn;
    QPushButton* mpMinBtn;
    QHBoxLayout* mpLayout;
};

class OwnToggleButton : public QAbstractButton {
    Q_OBJECT
    Q_PROPERTY(int mOffset READ offset WRITE setOffset)
public:
    explicit OwnToggleButton(int trackRadius, int thumbRadius, QWidget* parent = nullptr);
    ~OwnToggleButton() override {}

    QSize sizeHint() const override;

signals:
    void toggle(bool);

protected:
    void paintEvent(QPaintEvent *) override;
    void resizeEvent(QResizeEvent*) override;
    void mouseReleaseEvent(QMouseEvent  *) override;
    void enterEvent(QEvent *event) override;
    void setChecked(bool checked);

    int offset();
    void setOffset(int value);

private:
    int mOffset;
    int mBaseOffset;
    int mMargin;
    int mTrackRadius;
    int mThumbRadius;
    float mOpacity;
    QPropertyAnimation* mAnimation;

    QHash<bool, qreal> mEndOffset;
    QHash<bool, QBrush> mTrackColor;
    QHash<bool, QBrush> mThumbColor;
};

class OwnColorCombox : public QPushButton
{
    Q_OBJECT
public:
    OwnColorCombox(QWidget *parent = Q_NULLPTR);
    ~OwnColorCombox() {}

    void setButtonIcon(QColor color);

private:
    QMenu* createColorMenu(const char *slot, const char *slotColorBoard);
    QIcon createColorIcon(QColor color);

signals:
    void colorChanged(QColor color);

private slots:
    void onColorChanged();           // 文本颜色设置
    void onShowColorBoard();         // 颜色板

private:
    QSize mIconSize;
};




}

#endif // OWN_BUTTON_H
