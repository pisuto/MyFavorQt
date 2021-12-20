#include "own_pagebar.h"
#include "own_config.h"

#include <QtGlobal>
#include <QLabel>
#include <QIntValidator>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QDebug>

namespace mf {

OwnPageBar::OwnPageBar(int maxPages, int blockSize, QWidget* parent) : QWidget(parent),
    mMaxPage(0),
    mpPrePageLabel(new QLabel),
    mpNxtPageLabel(new QLabel),
    mpLeftPages(new QWidget),
    mpLeftSepLabel(new QLabel),
    mpCenterPages(new QWidget),
    mpRightSepLabel(new QLabel),
    mpRightPages(new QWidget),
    mpText(new QLabel),
    mpPageEdit(new QLineEdit),
    mpLayout(new QHBoxLayout)
{
    setBlockSize(blockSize);
    initialize();
    setMaxPage(maxPages);

    // 配置layout
    mpLayout->addStretch(1);
    mpLayout->addWidget(mpPrePageLabel);
    mpLayout->addWidget(mpLeftPages);
    mpLayout->addWidget(mpLeftSepLabel);
    mpLayout->addWidget(mpCenterPages);
    mpLayout->addWidget(mpRightSepLabel);
    mpLayout->addWidget(mpRightPages);
    mpLayout->addWidget(mpNxtPageLabel);
    mpLayout->addWidget(mpPageEdit);
    mpLayout->addWidget(mpText);
    mpLayout->addStretch(1);
    this->setLayout(mpLayout);
}

bool OwnPageBar::eventFilter(QObject *watched, QEvent *e)
{
    if(e->type() == QEvent::MouseButtonRelease)
    {
        int page = -1;
        if(watched == mpPrePageLabel)
        {
            page = getCurrentPage() - 1;
        }
        if(watched == mpNxtPageLabel)
        {
            page = getCurrentPage() + 1;
        }

        for (int i = 0; i < mlpPageLabels.count(); ++i)
        {
            if(watched == mlpPageLabels.at(i))
            {
                page = mlpPageLabels.at(i)->text().toInt();
                break;
            }
        }

        if(page != -1)
        {
            setCurrentPage(page, true);
            return true;
        }
    }

    if(watched == mpPageEdit && e->type() == QEvent::KeyRelease)
    {
        auto ke = static_cast<QKeyEvent*>(e);
        if(ke->key() == Qt::Key_Enter || ke->key() == Qt::Key_Return)
        {
            setCurrentPage(mpPageEdit->text().toInt(), true);
            return true;
        }
    }

    return QWidget::eventFilter(watched, e);
}

int OwnPageBar::getBlockSize() const {
    return mBlockSize;
}

int OwnPageBar::getMaxPage() const {
    return mMaxPage;
}

int OwnPageBar::getCurrentPage() const {
    return mCurrPage;
}

void OwnPageBar::setMaxPage(int page) {
    page = qMax(page, 1);

    if (mMaxPage != page) {
        this->mMaxPage = page;
        this->mCurrPage = 1;
        updatePageLabels();
    }
}

void OwnPageBar::setCurrentPage(int page, bool signalEmitted)
{
    page = qMax(page, 1);
    page = qMin(page, mMaxPage);

    if (page != this->mCurrPage) {
        this->mCurrPage = page;
        updatePageLabels();

        if (signalEmitted) {
            emit currentPageChanged(page);
        }
    }
}

void OwnPageBar::setBlockSize(int blockSize)
{
    // 为了便于计算, block size 必须是奇数, 且最小为3
    blockSize = qMax(blockSize, 3);
    if (blockSize % 2 == 0) {
        ++blockSize;
    }
    mBlockSize = blockSize;
}

// 初始化页码的labels
// 分成三个部分, 左...中...右
void OwnPageBar::initialize()
{
    mpText->setText(QString("page"));
    auto config = OwnConfig::getInstance();
#ifdef AUTO_SETTING_LOAD
    const auto& component = config->getSettingData().pagebar;
    const auto& font = component.fonts[0];
    mpText->setFont(QFont(font.name.c_str(), font.size, font.weight));
#else
    config->setFont(mpText, 9, QFont::Thin);
#endif
    mpLeftSepLabel->setText("..");
    mpRightSepLabel->setText("..");
    mpPrePageLabel->setText("<<");
    mpNxtPageLabel->setText(">>");

    mpPageEdit->installEventFilter(this);
    mpPageEdit->setObjectName("own_pagebar_edit");
    mpPageEdit->setPlaceholderText("number");
    mpPageEdit->setAlignment(Qt::AlignLeft);
    mpPageEdit->setFixedWidth(config->getDisplayImageSize().width() / 3);
    mpPageEdit->setValidator(new QIntValidator(1, 100, this));

    mpNxtPageLabel->setProperty("page", "true");
    mpPrePageLabel->setProperty("page", "true");
    mpNxtPageLabel->installEventFilter(this);
    mpPrePageLabel->installEventFilter(this);

    auto leftLayout = new QHBoxLayout();
    auto centerLayout = new QHBoxLayout();
    auto rightLayout = new QHBoxLayout();
    leftLayout->setContentsMargins(0, 0, 0, 0);
    leftLayout->setSpacing(0);
    centerLayout->setContentsMargins(0, 0, 0, 0);
    centerLayout->setSpacing(0);
    rightLayout->setContentsMargins(0, 0, 0, 0);
    rightLayout->setSpacing(0);

    for (int i = 0; i < mBlockSize * 3; ++i) {
        QLabel *label = new QLabel(QString::number(i + 1));
        label->setProperty("page", "true");
        label->installEventFilter(this);

        mlpPageLabels.append(label);

        if (i < mBlockSize) {
            leftLayout->addWidget(label);
        } else if (i < mBlockSize * 2) {
            centerLayout->addWidget(label);
        } else {
            rightLayout->addWidget(label);
        }
    }

    mpLeftPages->setLayout(leftLayout);
    mpCenterPages->setLayout(centerLayout);
    mpRightPages->setLayout(rightLayout);
}

void OwnPageBar::updatePageLabels()
{
    mpLeftSepLabel->hide();
    mpRightSepLabel->hide();

    if(mMaxPage <= mBlockSize * 3)
    {
        for(int i = 0; i < mlpPageLabels.count(); ++i)
        {
            auto label = mlpPageLabels.at(i);
            if(i < mMaxPage)
            {
                label->setText(QString::number(i + 1));
                label->show();
            }
            else {
                label->hide();
            }

            if (mCurrPage - 1 == i)
            {
                label->setProperty("currentPage", "true");
            } else {
                label->setProperty("currentPage", "false");
            }
            label->setStyleSheet("/**/");
        }
        return;
    }

    // 以下情况为maxPageNumber大于blockSize * 3, 所有的页码label都要显示
    // c 为 currentPage
    // n 为 block size
    // m 为 maxPage

    // 1. c ∈ [1, n + n/2 + 1]: 显示前 n * 2 个, 后 n 个: 只显示右边的分隔符
    // 2. c ∈ [m - n - n/2, m]: 显示前 n 个, 后 n * 2 个: 只显示左边的分隔符
    // 3. 显示[1, n], [c - n/2, c + n/2], [m - 2*n + 1, m]: 两个分隔符都显示

    int c = mCurrPage;
    int n = mBlockSize;
    int m = mMaxPage;
    int centerStartPage = 0;

    if (c >= 1 && c <= n + n / 2 + 1) {
        // 1. c ∈ [1, n + n/2 + 1]: 显示前 n * 2 个, 后 n 个: 只显示右边的分隔符
        centerStartPage = n + 1;
        mpRightSepLabel->show();
    } else if (c >= m - n - n / 2 && c <= m) {
        // 2. c ∈ [m - n - n/2, m]: 显示前 n 个, 后 n * 2 个: 只显示左边的分隔符
        centerStartPage = m - n - n + 1;
        mpLeftSepLabel->show();
    } else {
        // 3. 显示[1, n], [c - n/2, c + n/2], [m - n + 1, m]: 两个分隔符都显示
        centerStartPage = c - n / 2;
        mpRightSepLabel->show();
        mpLeftSepLabel->show();
    }

    for (int i = 0; i < n; ++i) {
        mlpPageLabels.at(i)->setText(QString::number(i + 1));                     // 前面 n 个
        mlpPageLabels.at(n + i)->setText(QString::number(centerStartPage + i));   // 中间 n 个
        mlpPageLabels.at(3 * n - i - 1)->setText(QString::number(m - i));         // 后面 n 个
    }

    for (int i = 0; i < mlpPageLabels.count(); ++i) {
        QLabel *label = mlpPageLabels.at(i);
        int page = label->text().toInt();
        if (page == mCurrPage) {
            label->setProperty("currentPage", "true");
        } else {
            label->setProperty("currentPage", "false");
        }

        label->setStyleSheet("/**/");
        label->show();
    }
}

}
