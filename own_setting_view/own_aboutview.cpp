#include "own_aboutview.h"

#include <QSplitter>
#include <QLabel>

namespace mf {

OwnBublePanel::OwnBublePanel(QWidget *parent) : QFrame(parent),
    mpSplitter(new QSplitter(Qt::Vertical))
{
    this->setAttribute(Qt::WA_TranslucentBackground);//设置背景透明
    this->setWindowFlags(Qt::FramelessWindowHint);
    this->setStyleSheet("QFrame{background-color:rgb(245,245,245);border:none;border-radius:8px;}");

    mpSplitter->setHandleWidth(1);
    mpSplitter->setStyleSheet("QSplitter::handle { background-color: rgba(200,200,200,80) }");
    auto pLayout = new QVBoxLayout();
    pLayout->addWidget(mpSplitter);
    pLayout->setContentsMargins(5, 5, 5, 5);
    this->setLayout(pLayout);
}

void OwnBublePanel::addWidget(QWidget* pItem)
{
    mpSplitter->addWidget(pItem);
}

void OwnBublePanel::finished()
{
    for (int i = 0; i < mpSplitter->count(); i++)
    {
        auto hndl = mpSplitter->handle(i);
        hndl->setCursor(Qt::ArrowCursor);
    }
}

QFrame* createInfoLabel(const QFont& font, const QString& prefix, const QString& link, float scale1 = 0.5f, float scale2 = 0.5f)
{
    auto pLayout = new QVBoxLayout();
    auto prefixLabel = new QLabel(prefix);
    prefixLabel->setFont(QFont(font.family(), static_cast<int>(font.pointSize() * scale1), QFont::Bold));
    auto linkLabel = new QLabel(link);
    linkLabel->setFont(QFont(font.family(), static_cast<int>(font.pointSize() * scale2)));

    pLayout->addWidget(prefixLabel);
    pLayout->addWidget(linkLabel);
    auto pFrame = new QFrame();
    pFrame->setLayout(pLayout);
    return pFrame;
}

OwnAboutView::OwnAboutView(int height, QFont font, QWidget *parent) : QFrame (parent),
    mpMainLayout(new QVBoxLayout)
{
    auto pPanel = new OwnBublePanel(this);
    pPanel->addWidget(createInfoLabel(font, "Github", "https://github.com/pisuto/MyFavorQt"));
    pPanel->addWidget(createInfoLabel(font, "Gitee", "https://gitee.com/wu_git/MyFavorQt"));
    pPanel->addWidget(createInfoLabel(font, "CSDN Blog", "https://blog.csdn.net/pisuto"));
    pPanel->finished();

    mpMainLayout->setContentsMargins(20, 10, 10, 20);
    /* 标题 */
    mpMainLayout->addWidget(createInfoLabel(font, "About", "For more infomation", 1, 0.5f));
    mpMainLayout->addWidget(pPanel);
    mpMainLayout->addStretch(1);
    this->setLayout(mpMainLayout);
}





}
