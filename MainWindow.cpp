/**
* This file is part of MyFavor
*
* Copyright (C) 2021-2022 Pisuto.
*
*/

#include "MainWindow.h"
#include "ui_MainWindow.h"

#include <QGraphicsDropShadowEffect>
#include <QVBoxLayout>
#include <QFrame>
#include <QPainter>
#include <QtMath>
#include <QDebug>

#define SHADOW_WIDTH 8		// 阴影边框宽度;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    mpMainWidget(new mf::OwnMainWidget(this))
{
    ui->setupUi(this);
    this->setWindowIcon(QIcon(":images/icon/ice.png"));
    //设置无边框透明
    this->setWindowFlags(Qt::FramelessWindowHint);//无边框
    this->setAttribute(Qt::WA_TranslucentBackground);//背景透明

    auto pBox = new QVBoxLayout(this->centralWidget());
    pBox->addWidget(mpMainWidget);
}

void MainWindow::paintEvent(QPaintEvent*)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.fillRect(QRect(SHADOW_WIDTH, SHADOW_WIDTH, this->width() - 2 * SHADOW_WIDTH, this->height() - 2 * SHADOW_WIDTH), QBrush(Qt::white));

    QColor color(50, 50, 50, 30);
    for (int i = 0; i < SHADOW_WIDTH; i++)
    {
        color.setAlpha(120 - qSqrt(i) * 40);
        painter.setPen(color);
        // 圆角阴影边框;
        painter.drawRoundedRect(SHADOW_WIDTH - i, SHADOW_WIDTH - i,
                                this->width() - (SHADOW_WIDTH - i) * 2,
                                this->height() - (SHADOW_WIDTH - i) * 2, 4, 4);
    }
}

void MainWindow::mouseMoveEvent(QMouseEvent* event)
{
    if (event->buttons() & Qt::LeftButton)
    {
        if(mDragPos.y() > mpMainWidget->getTopRightDownPos().height())
            return;
        auto desktopRc = QApplication::desktop()->availableGeometry();
        auto curPoint = event->globalPos() - mDragPos;
        if (event->globalY() > desktopRc.height())
        {
            curPoint.setY(desktopRc.height() - mDragPos.y());
        }
        this->move(curPoint);
    }
    QWidget::mouseMoveEvent(event);
}

void MainWindow::mousePressEvent(QMouseEvent*event)
{
    if (event->button() == Qt::LeftButton)
    {
        mDragPos = QCursor::pos() - frameGeometry().topLeft();
    }
    QWidget::mousePressEvent(event);
}

MainWindow::~MainWindow()
{
    delete ui;
}
