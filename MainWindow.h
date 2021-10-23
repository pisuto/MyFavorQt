#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVBoxLayout>
#include <QHBoxLayout>

#include <QPushButton>
#include <QVector>
#include <QListWidget>
#include <QLabel>

#include "own_element.h"
#include "own_stackedview.h"
#include "own_button.h"

class QFrame;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    QFrame* createContainer(QString path, QString title, QString author);

public slots:
    void on_btn1_clicked();
    void on_btn2_clicked();
    void on_btn3_clicked();

private:
    Ui::MainWindow *ui;
    QVBoxLayout* pMainLayout;
    QHBoxLayout* pTopLayout;
    QGridLayout* pListLayout;

    QListWidget* pImgList;
    QVector<QObject*> vpButtons;
    QVector<QListWidgetItem*> vpImgs;

    QVector<mf::OwnElement*> mvpElements;
    mf::OwnStackedView* mpStackedView;
    mf::OwnButtonGroup* mpBtnGrp;
};

#endif // MAINWINDOW_H
