/**
* This file is part of MyFavor
*
* Copyright (C) 2021-2022 Pisuto.
*
*/

#ifndef OWN_ABOUTVIEW_H
#define OWN_ABOUTVIEW_H

#include <QFrame>
#include <QVBoxLayout>

#include "own_addlabelview.h"

class QSplitter;

namespace mf {

class OwnMultiLabels;

class OwnBublePanel : public QFrame {
    Q_OBJECT
public:
    explicit OwnBublePanel(QWidget* parent = Q_NULLPTR);
    ~OwnBublePanel() {}

    void addWidget(QWidget*);
    void finished();
private:
    QSplitter* mpSplitter;
};


class OwnAboutView : public QFrame {
    Q_OBJECT
public:
    explicit OwnAboutView(int height, QFont font, QWidget* parent = Q_NULLPTR);
    ~OwnAboutView() {}

private:
    QVBoxLayout* mpMainLayout;

};


class OwnSystemView : public QFrame {
    Q_OBJECT /* https://q.cnblogs.com/q/43654/ */
public:
    explicit OwnSystemView(int height, QFont font, QWidget* parent = Q_NULLPTR);
    ~OwnSystemView() {}

    QFrame* createApplyButton(int size, const QFont& font, const QString& prefix, float scale = 0.5f);
    QFrame* createToggleButton(int thumb, int track, const QFont& font, const QString& prefix, float scale = 0.5f);
    QFrame* createColorCombox(const QFont& font, const QString& prefix, float scale = 0.5f);
    QFrame* createDropCombox(const QFont& font, const QString& prefix, float scale = 0.5f);
    QFrame* createMultiLabels();

public slots:
    void recvMsgFromDialog(QString name, QString path);
    void recvMsgFromColorBox(QColor color);
    void recvMsgFromApplyButton();

signals:
    void sendMsgToObj(QString, QString);

private:
    mf::OwnAddLabelView* mpDialog;
    QVBoxLayout* mpMainLayout;
};


}









#endif
