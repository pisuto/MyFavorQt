#ifndef OWN_ELEMENT_H
#define OWN_ELEMENT_H

#include <QFrame>

class QLabel;
class QImage;
class QPushButton;
class QGridLayout;

namespace mf {

class OwnElement : public QFrame
{
    Q_OBJECT
public:
    OwnElement(QString fileName, QString title, QString desc);

private:
    QImage* mpImg;
    QLabel* mpLable;
    QVector<QPushButton*> mvpLabels;
    QGridLayout* mpLayout;

    QString mTitle;
    QString mDesc;
    int mCategory;
};


}
#endif // OWN_ELEMENT_H
