#ifndef OWN_STACKEDVIEW_H
#define OWN_STACKEDVIEW_H

#include <QStackedWidget>
#include <QVariant>
#include <QThread>

class QPainter;
class QWidget;
class QPropertyAnimation;

// https://github.com/NoHearting/MySelfQQ_v2/blob/419bf3981ccf0e0c9f4c8bf7d6330e85beadd59c/feature_widgets/AnimationStackedWidget.h
namespace mf {

class OwnStackedView : public QStackedWidget
{
    Q_OBJECT
public:
    explicit OwnStackedView(QWidget* parent = nullptr);
    virtual ~OwnStackedView() {}

    void nextWidget();
    void previousWidget();
    void setDuration(quint16 dura) { mDuration = dura; }

public slots:
    void valueChangedAnimation(QVariant value);
    void animationFininshed();

protected:
    void paintEvent(QPaintEvent* event);

private:
    void paintPrevious(QPainter& painter, int index);
    void paintNext(QPainter& painter, int index);

    void startAnimation(int index);

private:
    QPropertyAnimation* mpAnime;
    QVariant mCurrentVal;
    quint16 mDuration;
    quint8 mWidgetCnt;
    quint8 mNextIdx;
    bool mbIsLeft;
    bool mbIsAnimation;

//    QThread* mpOwnThread;

};

}

#endif // OWN_STACKEDVIEW_H
