/**
* This file is part of MyFavor
*
* Copyright (C) 2021-2022 Pisuto.
*
*/

#include "own_util.h"
#include "own_database.h"
#include "own_database_item.h"
#include "../viewer/own_element.h"

#include <QFrame>
#include <QGridLayout>
#include <QStackedWidget>
#include <QDir>
#include <QFileInfo>
#include <QPainter>
#include <QDebug>
#include <QMessageBox>

namespace mf {

float OwnUtil::triangleFunc(float x)
{
    if(x <= 0.5f)
        return x;
    else if(x > 0.5f && x < 1.0f)
        return 1 - x;
    return 0.f;
}

bool OwnUtil::isFileExist(const QString& fileName)
{
    QFileInfo info(fileName);
    if(info.exists() && info.isFile())
       return true;
    return false;
}

void OwnUtil::createFile(const QString &fileName)
{
    if(!isFileExist(fileName))
    {
        QFile file(fileName);
        file.open(QIODevice::WriteOnly);
        file.close();
    }
}

bool OwnUtil::deleteFile(const QString &path)
{
    if(path.isEmpty())
        return false;
    QFileInfo info(path);
    if(info.isFile())
        return QFile::remove(path);
    return false;
}

bool OwnUtil::copyFile(const QString& source, const QString& target)
{
    if(source == target)
        return true;
    if(!isFileExist(source) || (isFileExist(target) && !deleteFile(target)))
        return false;
    if(!QFile::copy(source, target))
        return false;
    return true;
}

bool OwnUtil::checkXmlExists()
{
    if(!mf::OwnUtil::isFileExist(MF_DEFAULT_XML))
    {
        QMessageBox::StandardButton reply;
        reply = QMessageBox::critical(Q_NULLPTR, "Error", "No default configuration found!", QMessageBox::Ok);
        return false;
    }
    else if(!mf::OwnUtil::isFileExist(MF_CUSTOM_XML))
    {
        if(!mf::OwnUtil::copyFile(MF_DEFAULT_XML, MF_CUSTOM_XML))
        {
            QMessageBox::StandardButton reply;
            reply = QMessageBox::critical(Q_NULLPTR, "Error", "Copy default configuration failed!", QMessageBox::Ok);
            return false;
        }
    }
    return true;
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
    auto pConfig = OwnConfig::getInstance();

    const auto& grid = pConfig->getSettingData().screen.grid;
    auto grids = static_cast<int>(grid.row * grid.col);

    auto count = pConfig->getCategoryCount()[category - 1];
    return count % grids == 0 ? count / grids : count / grids + 1;
}

QFrame *OwnUtil::createNewPage(int start, int category, QString defaultPath, QString defaultTitle, QString defaultAuthor, QString defaultDesc)
{
    auto pDataBase = OwnDatabase::getInstance();
    auto pTmpLayout = new QGridLayout();

    const auto grid = OwnConfig::getInstance()->getSettingData().screen.grid;
    int row = static_cast<int>(grid.row);
    int col = static_cast<int>(grid.col);

    auto list = pDataBase->selectOnePage(start, row * col, category);
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
                elem->setVisible(0);
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
    auto pages = getPages(category);
    const auto& grid = OwnConfig::getInstance()->getSettingData().screen.grid;
    auto grids = static_cast<int>(grid.row * grid.col);

    if(pages == 0) pages = 1;
    for(int i = 0; i < pages; ++i)
    {
        auto pPage = createNewPage(i * grids, category);
        pWidget->addWidget(pPage);
    }
}

void OwnUtil::updatePages(QStackedWidget *pWidget, odbitem& item, int category, int id, SQL_ITEM_OPER oper)
{
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

    // ??????????????????????????????????????????????????????
    OwnElement* elem = Q_NULLPTR;
    const auto grid = OwnConfig::getInstance()->getSettingData().screen.grid;
    int row = static_cast<int>(grid.row);
    int col = static_cast<int>(grid.col);

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

    // ??????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????
    if(i == row && j == col && SQL_ITEM_OPER::INSERT == oper)
    {
        int start = (getPages(category) - 1) * row * col;
        pWidget->addWidget(createNewPage(start, category));
    }
    else if(SQL_ITEM_OPER::INSERT == oper || SQL_ITEM_OPER::UPDATE == oper)
    {
        elem->setValue(item);
    }
    else if(SQL_ITEM_OPER::DELETE == oper)
    {
        // ??????????????????
        OwnDatabase::getInstance()->storeDeletingItem(item);
        elem->deleting();
    }
    elem->setVisible(1);
}

QString OwnUtil::strAutoFeed(const QString &text, const QFont& font, int row, int px)
{
    QString str = text;
    QFontMetrics fm(font);
    int autoIdx = 1;

    if(!str.isEmpty())
    {
        // ????????????????????????????????????
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
