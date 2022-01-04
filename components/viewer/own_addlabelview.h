/**
* This file is part of MyFavor
*
* Copyright (C) 2021-2022 Pisuto.
*
*/

#ifndef OWN_ADDLABELVIEW_H
#define OWN_ADDLABELVIEW_H

#include <QDialog>
#include <QLayout>

class QFrame;
class QComboBox;
class QLineEdit;

namespace Ui {
class OwnAddLabelView;
}

namespace mf {

class OwnAddLabelView : public QDialog
{
    Q_OBJECT
public:
    explicit OwnAddLabelView(QWidget *parent = nullptr);
    ~OwnAddLabelView();

public slots:
    void confirm();

signals:
    void confirmMsg(QString, QString);

private:
    QFrame* createDropImageBox();

    Ui::OwnAddLabelView *ui;
    QVBoxLayout* mpLayout;
    QLineEdit* mpLineEdit;
    QComboBox* mpCombox;
};

}

#endif // OWN_ADDLABELVIEW_H
