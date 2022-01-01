#ifndef OWN_ABOUTVIEW_H
#define OWN_ABOUTVIEW_H

#include <QFrame>
#include <QVBoxLayout>

#include "own_addlabelview.h"

class QSplitter;

namespace mf {

class OwnBublePanel : public QFrame {

public:
    explicit OwnBublePanel(QWidget* parent = Q_NULLPTR);
    ~OwnBublePanel() {}

    void addWidget(QWidget*);
    void finished();
private:
    QSplitter* mpSplitter;
};


class OwnAboutView : public QFrame {

public:
    explicit OwnAboutView(int height, QFont font, QWidget* parent = Q_NULLPTR);
    ~OwnAboutView() {}

private:
    QVBoxLayout* mpMainLayout;

};


class OwnSystemView : public QFrame {

public:
    explicit OwnSystemView(int height, QFont font, QWidget* parent = Q_NULLPTR);
    ~OwnSystemView() {}

    QFrame* createDropCombox(const QFont& font, const QString& prefix, float scale = 0.5f);
    QFrame* createMultiLabels();
signals:
    void dropComboxChanged(QString);

private:
    OwnAddLabelView* mpDialog;
    QVBoxLayout* mpMainLayout;
};


}









#endif
