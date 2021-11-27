#include "own_util.h"
#include "own_database.h"
#include "own_database_item.h"
#include "own_element.h"

#include <QFrame>
#include <QGridLayout>
#include <QStackedWidget>
#include <QDir>
#include <QFileInfo>
#include <QPainter>
#include <QDebug>

namespace mf {

float OwnUtil::triangleFunc(float x)
{
    if(x <= 0.5f)
        return x;
    else if(x > 0.5f && x < 1.0f)
        return 1 - x;
    return 0.f;
}

bool OwnUtil::deleteFile(const QString &path)
{
    if(path.isEmpty() || !QDir().exists(path))
        return false;
    QFileInfo info(path);
    if(info.isFile())
        return QFile::remove(path);
    return false;
}

QImage OwnUtil::getImgWithOverlay(const QImage &base, const QImage &overlay)
{
    QImage imageWithOverlay = QImage(base.size(), QImage::Format_ARGB32_Premultiplied);
    QPainter painter(&imageWithOverlay);

    painter.setCompositionMode(QPainter::CompositionMode_Source);
    painter.fillRect(imageWithOverlay.rect(), Qt::transparent);

    painter.setCompositionMode(QPainter::CompositionMode_SourceOver);
    painter.drawImage(0, 0, base);

    painter.setCompositionMode(QPainter::CompositionMode_SourceOver);
    painter.drawImage(0, 0, overlay);

    painter.end();

    return imageWithOverlay;
}

int OwnUtil::getPages(int category)
{
    auto count = OwnConfig::getInstance()->getCategoryCount()[category - 1];
    auto grids = static_cast<int>(SQL_PAGE_ITEM_GRID::COUNT);
    return count % grids == 0 ? count / grids : count / grids + 1;
}

QFrame *OwnUtil::createNewPage(int start, int category, QString defaultPath, QString defaultTitle, QString defaultAuthor, QString defaultDesc)
{
    auto pDataBase = OwnDatabase::getInstance();
    auto list = pDataBase->selectOnePage(start, static_cast<SQL_ITEM_CATEGORY>(category));
    auto pTmpLayout = new QGridLayout();
    int row = static_cast<int>(mf::SQL_PAGE_ITEM_GRID::ROW);
    int col = static_cast<int>(mf::SQL_PAGE_ITEM_GRID::COL);
    for(int i = 0; i < row; ++i)
    {
        for(int j = 0; j < col; ++j)
        {
            int index = row * i + j;
            OwnElement* elem = Q_NULLPTR;
            if(list.size() > index) {
                auto item = list[index];
                elem = new OwnElement(SQL_TABLE_ITEM::ImgFileLocation() + "/" + item.path.value(),
                                      item.title.value(), item.author.value(),
                                      item.desc.value());
                elem->setId(item.id.value());
            }
            else {
                elem = new OwnElement(defaultPath, defaultTitle, defaultAuthor, defaultDesc);
                elem->setElemOpacity(0);
            }
            elem->setCategory(category);
            pTmpLayout->addWidget(elem, i, j, 1, 1);
        }
    }
    pTmpLayout->setMargin(5);
    auto container = new QFrame();
    container->setLayout(pTmpLayout);
    return container;
}

void OwnUtil::addPages(QStackedWidget *pWidget, int category)
{
    auto pDataBase = OwnDatabase::getInstance();
    auto grids = static_cast<int>(SQL_PAGE_ITEM_GRID::COUNT);
    auto pages = getPages(category);

    if(pages == 0) pages = 1;
    for(int i = 0; i < pages; ++i)
    {
        auto pPage = createNewPage(i * grids, category);
        pWidget->addWidget(pPage);
    }
}

void OwnUtil::updatePages(QStackedWidget *pWidget, int category, int id, SQL_ITEM_OPER oper)
{
    // 获取新插入的数据
    auto pDataBase = OwnDatabase::getInstance();
    odbitem item;
    item.id = id;
    item = pDataBase->select(item);
    if(item.category.value() != category) // 如果不是一个类别的直接返回
    {
        return;
    }

    QGridLayout* pLayout = Q_NULLPTR;
    if(SQL_ITEM_OPER::INSERT == oper)
    {
        auto pLastWidget = qobject_cast<QFrame*>(pWidget->widget(pWidget->count() - 1));
        pLayout = qobject_cast<QGridLayout*>(pLastWidget->layout());
    }
    else
    {
        auto pCurrWidget = qobject_cast<QFrame*>(pWidget->currentWidget());
        pLayout = qobject_cast<QGridLayout*>(pCurrWidget->layout());
    }

    // 寻找空的元素或者是待更新和删除的元素
    OwnElement* elem = Q_NULLPTR;
    int row = static_cast<int>(mf::SQL_PAGE_ITEM_GRID::ROW);
    int col = static_cast<int>(mf::SQL_PAGE_ITEM_GRID::COL);
    int i = 0, j = 0;
    bool flag = false;
    for(; i < row; ++i)
    {
        for(j = 0; j < col; ++j)
        {
            elem = qobject_cast<OwnElement*>(pLayout->itemAt(col * i + j)->widget());
            if(elem &&
                    ((elem->getId() == -1 && SQL_ITEM_OPER::INSERT == oper) ||
                     (elem->getId() == id && (SQL_ITEM_OPER::UPDATE == oper || SQL_ITEM_OPER::DELETE == oper)))) {
                flag = true;
                break;
            }
        }
        if(flag) {
            break;
        }
    }

    // 如果已经满了则需要另外新增页面，否则直接更新当前元素，最后或者是删除元素更新整个界面
    if(i == row && j == col && SQL_ITEM_OPER::INSERT == oper)
    {
        int start = getPages(category) * row * col;
        pWidget->addWidget(createNewPage(start, category));
    }
    else if(SQL_ITEM_OPER::INSERT == oper || SQL_ITEM_OPER::UPDATE == oper)
    {
        elem->setValue(item);
    }
    else if(SQL_ITEM_OPER::DELETE == oper)
    {
        // 更新整体页面
        pDataBase->storeDeletingItem(item);
        elem->deleting();
    }
    elem->setElemOpacity(1);
}

QString OwnUtil::strAutoFeed(const QString &text, const QFont& font, int row, int px)
{
    QString str = text;
    QFontMetrics fm(font);
    int autoIdx = 1;

    if(!str.isEmpty())
    {
        // 得到可以整除的字符串宽度
        int realPx = (px / fm.width(str.left(1))) * fm.width(str.left(1));
        for(int i = 0; i < str.size(); ++i)
        {
            auto width = fm.width(str.left(i));
            if(width >= realPx * autoIdx)
            {
                autoIdx++;
                str.insert(i, "\n");
            }
            if(width >= realPx * row)
            {
                str.remove(i - 3, str.length() - i + 3);
                str.insert(i - 3, "...\n");
                break;
            }
        }
    }
    return str;
}

}
