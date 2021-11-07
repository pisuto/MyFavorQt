#ifndef OWN_SLIDESTACKEDVIEW_H
#define OWN_SLIDESTACKEDVIEW_H

#include <QStackedWidget>
#include <QVariant>
#include <QThread>

#include "item_upload_window/own_itemuploadview.h"

class QPainter;
class QWidget;
class QPropertyAnimation;

// https://github.com/NoHearting/MySelfQQ_v2/blob/419bf3981ccf0e0c9f4c8bf7d6330e85beadd59c/feature_widgets/AnimationStackedWidget.h
namespace mf {

class OwnSlideStackedView : public QStackedWidget
{
    Q_OBJECT
public:
    explicit OwnSlideStackedView(QWidget* parent = nullptr);
    virtual ~OwnSlideStackedView() {}

    void nextWidget(int nxtIdx);
    void setDuration(quint16 dura) { mDuration = dura; }

public slots:
    void valueChangedAnimation(QVariant value);
    void animationFininshed();

    void deleteOperAction();
    void addOperAction();

protected:
    void paintEvent(QPaintEvent* event);

private:
    void paintPrevious(QPainter& painter, int index);
    void paintNext(QPainter& painter, int index);

    void startAnimation(int index);
    void initRightMenu();

private:
    QPropertyAnimation* mpAnime;
    QVariant mCurrentVal;
    quint16 mDuration;
    quint8 mWidgetCnt;
    quint8 mNextIdx;
    bool mbIsLeft;
    bool mbIsAnimation;

    OwnItemUploadView* mpUploadView;
};

}

#endif // OWN_STACKEDVIEW_H