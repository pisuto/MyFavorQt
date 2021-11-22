#include "own_lineedit.h"

#include <QEvent>

namespace mf {

void OwnLineEdit::setNullStyle()
{
    this->setStyleSheet("QLineEdit{border:1.5px solid red; "
                        "border-radius: 3px; padding-left: 3px;}");
    mbIsNull = true;
}

bool OwnLineEdit::eventFilter(QObject *watched, QEvent *event)
{
    if(this == watched)
    {
        if(event->type() == QEvent::FocusIn)
        {
            this->setStyleSheet("QLineEdit{border:1.5px solid blue; "
                                "border-radius: 3px; padding-left: 3px;}");
            mbIsNull = false;
        }
        else if(event->type() == QEvent::FocusOut && !mbIsNull)
        {
            this->setStyleSheet("QLineEdit{border:1px solid #A0A0A0; "
                                "border-radius: 3px; padding-left: 3px;}");
        }
    }
    return QLineEdit::eventFilter(watched, event);
}



}
