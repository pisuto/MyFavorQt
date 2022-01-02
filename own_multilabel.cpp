#include "own_multilabels.h"
#include "own_setting_view/own_addlabelview.h"
#include "own_config.h"

#include <QSignalMapper>
#include <QMessageBox>
#include <QDebug>

namespace mf {

OwnLabel::OwnLabel(const QString& text, QWidget* parent) : QFrame (parent), mpLabel(new QLabel(text)),
    mpCross(new QPushButton), mpParent(nullptr),
    mpMapper(new QSignalMapper(this)), mpLayout(new QHBoxLayout)
{
    this->setFixedSize(90, 30);
    mpCross->setIcon(QIcon(":/images/windows/cancel-cross.png"));
    mpCross->setFixedSize(16, 16);
    mpCross->setIconSize(QSize(10, 10));

    auto frameStyle = QString("QFrame{background-color:rgb(245,245,245);border:2px solid #dcdfe6;border-radius:3px;}");
    auto labelStyle = QString("QLabel{font-family:'Microsoft YaHei';font-size:14px;color:rgb(150,150,150);border:none;}");
    auto buttonStyle = QString("QPushButton{background-color:rgb(245, 245, 245);border-radius:3px;}"
                         "QPushButton:hover{background-color:rgb(235, 235, 235);}"
                         "QPushButton:pressed{background-color:rgb(200, 200, 200);}");
    this->setStyleSheet(frameStyle);
    mpLabel->setStyleSheet(labelStyle);
    mpCross->setStyleSheet(buttonStyle);

    mpLayout->addWidget(mpLabel);
    mpLayout->addWidget(mpCross);
    mpLayout->setContentsMargins(5, 3, 5, 3);
    this->setLayout(mpLayout);

    connect(mpCross, SIGNAL(clicked()), mpMapper, SLOT(map()));
    mpMapper->setMapping(mpCross, mIndex);
}

void OwnLabel::updateMapping()
{
    mpMapper->setMapping(mpCross, mIndex);
}

void OwnLabel::setParentWidget(OwnMultiLabels *labels)
{
    mpParent = labels;
    connect(mpMapper, SIGNAL(mapped(int)), mpParent, SLOT(removeLabel(int)));
}

OwnMultiLabels::OwnMultiLabels(QWidget* parent) : QFrame(parent),
   mCol(4), mPos{0, 0}, mpLayout(new QGridLayout)
{
    this->setFixedWidth((90 + 5) * mCol);
    mpLayout->setSizeConstraint(QLayout::SetFixedSize);
    mpLayout->setContentsMargins(1, 1, 1, 1);
    this->setLayout(mpLayout);
}

void OwnMultiLabels::addLabel(OwnLabel *item)
{
    item->setParentWidget(this);
    item->index() = mPos.row * mCol + mPos.col + 1;
    item->updateMapping();
    mpLayout->addWidget(item, mPos.row, mPos.col, 1, 1);
    if(++mPos.col >= mCol) {
        mPos.col = 0; mPos.row++;
    }
}

void OwnMultiLabels::removeLabel(int index) /* 第几个 */
{
    if(index <= 0) return;
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Deleted", "Please be aware this operation may cause data loss.", QMessageBox::Yes | QMessageBox::No);
    if(reply == QMessageBox::Yes) {
        int row = (index - 1) / mCol;
        int col = (index - 1) % mCol;
        /* 删除索引所在的元素 */
        {
            auto item = mpLayout->itemAtPosition(row, col)->widget();
            auto pos = mpLayout->indexOf(item);
            auto own = mpLayout->takeAt(pos);
            if(own) {
                own->widget()->hide();
                mpLayout->removeItem(own);
                own->widget()->deleteLater();
                delete own;
                own = Q_NULLPTR;
            }
            /* 删除全局数据 */
            OwnConfig::getInstance()->handler(this, HANDLER_OPER::OPER_DEL, index - 1);
        }
        /* 重新排序 */
        if(mPos.col == 0) {
            mPos.col = mCol - 1;
            mPos.row--;
        }
        else {
            mPos.col--;
        }
        if(row == mPos.row && col == mPos.col)
        {
            return; /* 删除的元素在最末尾，直接返回 */
        }
        else
        {
            QList<QLayoutItem*> items;
            QLayoutItem* child;
            while((child = mpLayout->takeAt(0)) != Q_NULLPTR) {
                items.push_back(child);
                auto item = qobject_cast<OwnLabel*>(child->widget());
                if(item && item->index() > index) {
                    item->index()--;
                    item->updateMapping();
                }
            }

            GridPos pos = {0, 0};
            for(auto& item : items) {
                mpLayout->addItem(item, pos.row, pos.col);
                if(++pos.col >= mCol) {
                    pos.col = 0; pos.row++;
                }
            }
        }
    }
}

void OwnMultiLabels::insertLabel(QString name, QString path)
{
    this->addLabel(new OwnLabel(name));

    /* 全局数据处理 */
    OwnConfig::getInstance()->handler(this, HANDLER_OPER::OPER_ADD, name, path);
}

}
