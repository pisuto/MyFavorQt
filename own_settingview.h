#ifndef OWN_SETTINGVIEW_H
#define OWN_SETTINGVIEW_H

#include <QFrame>
#include <QVBoxLayout>

#include "own_button.h"
#include "own_fadestackedview.h"

namespace mf {

class OwnSettingView : public QFrame
{
    Q_OBJECT
public:
    explicit OwnSettingView(QWidget* parent = Q_NULLPTR);
    ~OwnSettingView() {}


private:
    QVBoxLayout* mpMainLayout;

    mf::OwnButtonGroup* mpBtnGrp;
    mf::OwnFadeStackedView* mpStackedView;
};

}

#endif // OWN_SETTINGVIEW_H
