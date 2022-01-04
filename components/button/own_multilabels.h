/**
* This file is part of MyFavor
*
* Copyright (C) 2021-2022 Pisuto.
*
*/

#ifndef OWN_MULTILABELS_H
#define OWN_MULTILABELS_H

#include <QFrame>
#include <QLabel>
#include <QPushButton>
#include <QLayout>

class QSignalMapper;

namespace mf {

class OwnMultiLabels;
class OwnAddLabelView;

class OwnLabel : public QFrame
{
    Q_OBJECT
public:
    explicit OwnLabel(const QString& text, QWidget* parent = Q_NULLPTR);
    ~OwnLabel() {}

    void setText(QString str) { mpLabel->setText(str); }
    QString text() const { return mpLabel->text(); }

    int& index() { return mIndex; }
    void updateMapping();

    void setParentWidget(OwnMultiLabels* labels);
    OwnMultiLabels* parentWidget() { return mpParent; }

private:
    int mIndex;
    QLabel* mpLabel;
    QPushButton* mpCross;
    OwnMultiLabels* mpParent;

    QSignalMapper* mpMapper;
    QHBoxLayout* mpLayout;
};


class OwnMultiLabels : public QFrame
{
    Q_OBJECT
public:
    explicit OwnMultiLabels(QWidget* parent = Q_NULLPTR);
    ~OwnMultiLabels() {}

    void addLabel(OwnLabel* item);

public slots:
    void removeLabel(int index); /* 只有通过信号槽才能删除 */
    void insertLabel(QString name, QString path);

private:
    int mCol;
    struct GridPos {
        int row, col;
    } mPos;
    QGridLayout* mpLayout;
};

}

#endif // OWN_MULTILABELS_H
