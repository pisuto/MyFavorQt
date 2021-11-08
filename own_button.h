#ifndef OWN_BUTTON_H
#define OWN_BUTTON_H

#include <QPushButton>
#include <QButtonGroup>
#include <QLayout>
#include <QVariant>
#include <QApplication>
#include <QMouseEvent>
#include <QDesktopWidget>

class QPropertyAnimation;

namespace mf {

class OwnButton : public QPushButton
{
    Q_OBJECT
public:
    explicit OwnButton(const int& index, const QString& text, QWidget* parent = nullptr) :
        QPushButton(text, parent), mIndex(index)
    {
        auto style = QString("QPushButton{background-color:rgba(220,220,220,200);border:none;font-family:'Microsoft YaHei';}"
                             "QPushButton:hover{background-color:rgba(211,211,211,200);}"
                             "QPushButton:pressed{background-color:rgba(105,105,105,200);}");
        this->setStyleSheet(style);
        connect(this, &QPushButton::clicked, this, &OwnButton::ownButtonClicked);
    }
    OwnButton(const int& index, const QIcon& icon, const QString &text, QWidget* parent = nullptr) :
        QPushButton(icon, text, parent), mIndex(index)
    {
        auto style = QString("QPushButton{background-color:rgba(255,255,255,40);border:none;font-family:'Microsoft YaHei';}"
                             "QPushButton:hover{background-color:rgba(211,211,211,40);}"
                             "QPushButton:pressed{background-color:rgba(105,105,105,80);}");
        this->setStyleSheet(style);
        connect(this, &QPushButton::clicked, this, &OwnButton::ownButtonClicked);
    }
    ~OwnButton() {}

    int geIndex() { return mIndex; }

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
    explicit OwnButtonGroup(int startPos, QWidget* parent = nullptr);
    ~OwnButtonGroup() {}

    void initButtonConnect();
    void addButton(OwnButton* pBtn);
    void addStretch(int flag) { mpLayout->addStretch(flag); }

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
    QHBoxLayout* mpLayout;

    int mCurIdx;
    int mPreIdx;
    QVariant mCurrVal;
    QSize mBtnSize;
    int mLineHeight;
    QColor mLineColor;

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

        connect(mpCloseBtn, &QPushButton::pressed, this, [](){ QApplication::quit(); });
        connect(mpMinBtn,   &QPushButton::pressed, this, [&](){ mpParent->showMinimized(); });
    }

    ~OwnTopButtonGroup() {}

private:
    QWidget* mpParent;
    QPushButton* mpCloseBtn;
    QPushButton* mpMinBtn;

    QHBoxLayout* mpLayout;
};

}

#endif // OWN_BUTTON_H
