#ifndef OWN_ABOUTVIEW_H
#define OWN_ABOUTVIEW_H

#include <QFrame>
#include <QVBoxLayout>

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


}









#endif
