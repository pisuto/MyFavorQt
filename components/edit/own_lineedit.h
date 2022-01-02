#ifndef OWN_LINEEDIT_H
#define OWN_LINEEDIT_H

#include <QLineEdit>

namespace mf {

class OwnLineEdit : public QLineEdit
{
    Q_OBJECT
public:
    explicit OwnLineEdit(QWidget* parent = Q_NULLPTR) : QLineEdit(parent), mbIsNull(false){
        this->installEventFilter(this);
        this->setFocusPolicy(Qt::ClickFocus);

        this->setStyleSheet("QLineEdit{border:1px solid #A0A0A0; "
                            "border-radius: 3px; padding-left: 3px;}");
    }

    void setNullStyle();

protected:
    bool eventFilter(QObject * watched, QEvent * event);

private:
    bool mbIsNull;
};


}

#endif // OWN_LINEEDIT_H
