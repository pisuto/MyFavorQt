/**
* This file is part of MyFavor
*
* Copyright (C) 2021-2022 Pisuto.
*
*/

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "components/viewer/own_mainwidget.h"

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

protected:
    void paintEvent(QPaintEvent *event);
    void mouseMoveEvent(QMouseEvent* event);
    void mousePressEvent(QMouseEvent*event);

private:
    Ui::MainWindow *ui;
    mf::OwnMainWidget* mpMainWidget;
    QPoint mDragPos;
};

#endif // MAINWINDOW_H
