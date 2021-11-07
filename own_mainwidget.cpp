#include "own_mainwidget.h"
#include "own_config.h"

#include <QPainter>
#include <QtMath>
#include <QMenu>

namespace mf {

OwnMainWidget::OwnMainWidget(QWidget *parent) :
    QFrame(parent),
    mpBtnGrp(new mf::OwnButtonGroup(this)),
    mpTopBtns(new mf::OwnTopButtonGroup(parent)),
    mpMenu(new QMenu(this))
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

    // 初始化页面的分页组件
    mpPageBar = new mf::OwnPageBar(OwnConfig::getInstance()->getPages()[0]);
    mpStackedView = new mf::OwnFadeStackedView(mpPageBar, this);
    // 显示元素
    QString path = "F:/Projects/Project.program/Qt/MyFavor/MyFavorQt/images/OIP-C.jpg";
    for(int i = 0; i < 3; i++)
    {
        auto container1 = createContainer(path, QString("title%1").arg(i*3),
                                          QString("user%1").arg(i*3),
                                          QString("desc%1").arg(i*3));
        auto container2 = createContainer(path, QString("title%1").arg(i*3+1),
                                          QString("user%1").arg(i*3+1),
                                          QString("desc%1").arg(i*3+1));
        auto container3 = createContainer(path, QString("title%1").arg(i*3+2),
                                          QString("user%1").arg(i*3+2),
                                          QString("desc%1").arg(i*3+2));
        auto view = new mf::OwnSlideStackedView();
        view->addWidget(container1);
        view->addWidget(container2);
        view->addWidget(container3);
        mpStackedView->addWidget(view);
    }

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
    for(int i = 0; i < static_cast<int>(mf::SQL_PAGE_ITEM_GRID::ROW); ++i)
    {
        for(int j = 0; j < static_cast<int>(mf::SQL_PAGE_ITEM_GRID::COL); ++j)
        {
            pTmpLayout->addWidget(new mf::OwnElement(path, title, author, desc), i, j, 1, 1);
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
