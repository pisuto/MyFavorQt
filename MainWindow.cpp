#include "MainWindow.h"
#include "ui_MainWindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    mpStackedView(new mf::OwnStackedView),
    mpBtnGrp(new mf::OwnButtonGroup)
{
    ui->setupUi(this);
    // 创建layout
    pMainLayout = new QVBoxLayout(this->centralWidget());

    // 创建buttons
    auto btn1 = new QPushButton("Anime");
    auto btn2 = new QPushButton("Movie");
    auto btn3 = new QPushButton("Music");
    auto btn4 = new QPushButton("Set");
    connect(btn1, &QPushButton::clicked,
            this, &MainWindow::on_btn1_clicked);
    connect(btn2, &QPushButton::clicked,
            this, &MainWindow::on_btn2_clicked);
    connect(btn3, &QPushButton::clicked,
            this, &MainWindow::on_btn3_clicked);
    // 添加buttons到layout里
    mpBtnGrp->addButton(btn1, 0);
    mpBtnGrp->addButton(btn2, 1);
    mpBtnGrp->addButton(btn3, 2);
    mpBtnGrp->addStretch(1);
    mpBtnGrp->addButton(btn4, 3);
//    pTopLayout->addStretch(1);
//    auto user = new QLabel("User");
//    user->setObjectName("user");
//    pTopLayout->addWidget(user);

    // 显示元素
    QString path = "F:/Projects/Project.program/Qt/MyFavor/images/OIP-C.jpg";
    auto container1 = createContainer(path, "", "");
    auto container2 = createContainer(path, "", "");
    auto container3 = createContainer(path, "", "");
    auto container4 = createContainer(path, "", "");
    mpStackedView->addWidget(container1);
    mpStackedView->addWidget(container2);
    mpStackedView->addWidget(container3);
    mpStackedView->addWidget(container4);

    // 设置边框
    pMainLayout->setMargin(0);
    // 设置所有layout之间的包含关系
    pMainLayout->addWidget(mpBtnGrp);
    pMainLayout->addWidget(mpStackedView);
}

QFrame* MainWindow::createContainer(QString path, QString title, QString author)
{
    auto pTmpLayout = new QGridLayout();
//    for(int i=0; i < 9; ++i)
//    {
//        auto tmpElem = new mf::OwnElement(path, title, author);
//        mvpElements.push_back(tmpElem);
//    }
    const int row = 3, col = 3;
    for(int i = 0; i < row; ++i)
    {
        for(int j = 0; j < col; ++j)
        {
//            pTmpLayout->addWidget(mvpElements[i*col+j], i, j);
            pTmpLayout->addWidget(new mf::OwnElement(path, title, author), i, j);
        }
    }
    pTmpLayout->setMargin(5);
    auto container = new QFrame();
    container->setLayout(pTmpLayout);
    return container;
}

void MainWindow::on_btn1_clicked()
{
    mpStackedView->nextWidget();
}

void MainWindow::on_btn2_clicked()
{
    mpStackedView->previousWidget();
}

void MainWindow::on_btn3_clicked()
{

}

MainWindow::~MainWindow()
{
    delete ui;
}
