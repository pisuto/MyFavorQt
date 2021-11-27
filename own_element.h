#ifndef OWN_ELEMENT_H
#define OWN_ELEMENT_H

#include <QFrame>

class QLabel;
class QImage;
class QPushButton;
class QGridLayout;
class QGraphicsOpacityEffect;

namespace mf {
    class odbitem;
}

namespace mf {

class OwnElement : public QFrame
{
    Q_OBJECT
public:
    OwnElement(QString fileName, QString title, QString user, QString desc);

    void setId(int id);
    int getId() const;

    void setCategory(int category);
    int getCategory() const;

    void setValue(odbitem& item);
    void reset();
    bool deleting();

    bool getDeletingStatus() const;
    void setDeletingStatus(bool value);

    void setElemOpacity(float value);

private:
    void initRightMenu();
    void updateImage(QString filename);
    void updateImage(QImage& tmpImg);

    QImage* mpImg;
    QLabel* mpLable;
    QLabel* mpTitle;
    QLabel* mpUser;
    QLabel* mpDesc;
    QVector<QPushButton*> mvpLabels;
    QGridLayout* mpLayout;
    QGraphicsOpacityEffect* mpOpacity;

    QString mTitle;
    QString mUser;
    QString mDesc;
    int mCategory;
    int mIdentification; /* 保存id用于获取数据，默认为-1 */
    bool mbDeleting;
};


}
#endif // OWN_ELEMENT_H
