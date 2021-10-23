#ifndef OWN_BUTTON_H
#define OWN_BUTTON_H

#include <QPushButton>
#include <QButtonGroup>
#include <QLayout>
#include <QVariant>

class QPropertyAnimation;

namespace mf {

class OwnButtonGroup : public QWidget
{
    Q_OBJECT
public:
    explicit OwnButtonGroup(QWidget* parent = nullptr);
    ~OwnButtonGroup() {}

    void addButton(QPushButton* pBtn, int id);
    void addStretch(int flag) { mpLayout->addStretch(flag); }

public slots:
    void valueChangedAnimation(QVariant value);
    void onButtonClicked(int index);

protected:
    void paintEvent(QPaintEvent *event);

private:
    int getCurrLength(int value);

private:
    QPropertyAnimation* mpAnime;
    QButtonGroup* mpBtnGrp;
    QHBoxLayout* mpLayout;

    int mCurIdx;
    int mPreIdx;
    QVariant mCurrVal;
    QSize mBtnSize;
    int mLineHeight;
    QColor mLineColor;

};

}

#endif // OWN_BUTTON_H
