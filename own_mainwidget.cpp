#include "own_mainwidget.h"

#include <QPainter>
#include <QtMath>

namespace mf {

OwnMainWidget::OwnMainWidget(QWidget *parent) :
    QFrame(parent),
    mpStackedView(new mf::OwnFadeStackedView(this)),
    mpBtnGrp(new mf::OwnButtonGroup(this)),
    mpPageBar(new mf::OwnPageBar),
    mpTopBtns(new mf::OwnTopButtonGroup(parent))
{
    // 创建layout
    pMainLayout = new QVBoxLayout();
    // 创建buttons
    auto btn1 = new mf::OwnButton(-1, QIcon(":/images/svgtopng/undo.png"), "");
    auto btn2 = new mf::OwnButton(0, QIcon(":/images/svgtopng/anime.png"), "Anime");
    auto btn3 = new mf::OwnButton(1, QIcon(":/images/svgtopng/manga.png"), "Manga");
    auto btn4 = new mf::OwnButton(2, QIcon(":/images/svgtopng/movie.png"), "Movie");
    auto btn5 = new mf::OwnButton(3, QIcon(":/images/svgtopng/music.png"), "Music");
    auto btn6 = new mf::OwnButton(4, QIcon(":/images/svgtopng/photos.png"), "Photo");
    auto btn7 = new mf::OwnButton(5, QIcon(":/images/svgtopng/setting.png"), "");
    btn1->setIconSize(QSize(30,30));
    btn1->setFixedSize(40, 30);
    btn2->setIconSize(QSize(20,20));
    btn3->setIconSize(QSize(20,20));
    btn4->setIconSize(QSize(20,20));
    btn5->setIconSize(QSize(20,20));
    btn6->setIconSize(QSize(20,20));
    btn7->setIconSize(QSize(20,20));
    connect(btn2, &QPushButton::clicked,
            this, &OwnMainWidget::on_btn2_clicked);
    connect(btn3, &QPushButton::clicked,
            this, &OwnMainWidget::on_btn3_clicked);
    connect(btn4, &QPushButton::clicked,
            this, &OwnMainWidget::on_btn4_clicked);
    // 添加buttons到layout里
    mpBtnGrp->setObjectName("own_buttongroup");
    mpBtnGrp->layout()->addWidget(btn1);
    mpBtnGrp->addButton(btn2);
    mpBtnGrp->addButton(btn3);
    mpBtnGrp->addButton(btn4);
    mpBtnGrp->addButton(btn5);
    mpBtnGrp->addButton(btn6);
    mpBtnGrp->addStretch(1);
    mpBtnGrp->addButton(btn7);
    mpBtnGrp->initButtonConnect();

    // 显示元素
    QString path = "F:/Projects/Project.program/Qt/MyFavor/MyFavorQt/images/OIP-C.jpg";
    auto container1 = createContainer(path, "title1", "user1", "desc1");
    auto container2 = createContainer(path, "title2", "user2", "desc2");
    auto container3 = createContainer(path, "title3", "user3", "desc3");
    mpStackedView->addWidget(container1);
    mpStackedView->addWidget(container2);
    mpStackedView->addWidget(container3);

    // 设置边框
    pMainLayout->setContentsMargins(5, 5, 5, 5);
    // 设置所有layout之间的包含关系
    pMainLayout->addWidget(mpTopBtns);
    pMainLayout->addWidget(mpBtnGrp);
    pMainLayout->addWidget(mpStackedView);
    pMainLayout->addWidget(mpPageBar);
    this->setLayout(pMainLayout);
}

QFrame* OwnMainWidget::createContainer(QString path, QString title, QString author, QString desc)
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
            pTmpLayout->addWidget(new mf::OwnElement(path, title, author, desc), i, j);
        }
    }
    pTmpLayout->setMargin(5);
    auto container = new QFrame();
    container->setLayout(pTmpLayout);
    return container;
}

void OwnMainWidget::on_btn2_clicked()
{
    mpStackedView->switchWidget(0);
}

void OwnMainWidget::on_btn3_clicked()
{
    mpStackedView->switchWidget(1);
}

void OwnMainWidget::on_btn4_clicked()
{
    mpStackedView->switchWidget(2);
}


}
