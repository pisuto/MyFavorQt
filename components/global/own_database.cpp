/**
* This file is part of MyFavor
*
* Copyright (C) 2021-2022 Pisuto.
*
*/

#include "own_database.h"
#include "own_database_item.h"
#include "own_util.h"

#include <QDebug>

namespace mf {

OwnDatabase::OwnDatabase()
{
    if (QSqlDatabase::contains("qt_sql_default_connection"))
    {
        mDBAllocator = QSqlDatabase::database("qt_sql_default_connection");
    }
    else
    {
        mDBAllocator = QSqlDatabase::addDatabase("QSQLITE");
        mDBAllocator.setDatabaseName(SQL_TABLE_ITEM::ITEM_DATABASE_NAME);
    }

    if(!mDBAllocator.open())
    {
        qDebug() << "ERROR: FAILED OPEN DATABASE " << mDBAllocator.lastError();
        return;
    }
    // 初始化
    mQueExecutor = QSqlQuery(mDBAllocator);
    mQueExecutor.prepare(SQL_SYNTAX::CREATE_ITEM_TABLE_SQL);
    if(!mQueExecutor.exec())
    {
        qDebug() << "ERROR: " << SQL_SYNTAX::CREATE_ITEM_TABLE_SQL << " "
                 << mQueExecutor.lastError();
    }
}

int OwnDatabase::insert(odbitem &item)
{
    mQueExecutor.prepare(SQL_SYNTAX::INSERT_ITEM_SQL);
    bindValues(item.title, item.author, item.desc, item.label, item.path,
               item.category, item.create_year);
    if(!mQueExecutor.exec())
    {
        qDebug() << "ERROR: " << SQL_SYNTAX::INSERT_ITEM_SQL << " "
                 << mQueExecutor.lastError();
    }
    return mQueExecutor.lastInsertId().toInt(); // 返回自增的主键值
}

odbitem OwnDatabase::select(odbitem &item)
{
    mQueExecutor.prepare(SQL_SYNTAX::SELECT_ITEM_SQL);
    bindValues(item.id);
    if(!mQueExecutor.exec())
    {
        qDebug() << "ERROR: " << SQL_SYNTAX::SELECT_ITEM_SQL << " "
                 << mQueExecutor.lastError();
    }
    else
    {
        while(mQueExecutor.next())
        {
            resetItemWithResult(item);
        }
    }
    return item;
}

QList<odbitem> OwnDatabase::selectOnePage(int pos, int count, int category)
{
    odbitem item;
    QList<odbitem> list;

    item.category = category;
    mQueExecutor.prepare(SQL_SYNTAX::SELECT_ITEM_POS_OFFSET_SQL.arg(count).arg(pos));
    bindValues(item.category);
    if(!mQueExecutor.exec())
    {
        qDebug() << "ERROR: " << SQL_SYNTAX::SELECT_ITEM_POS_OFFSET_SQL.arg(pos).arg(count - 1) << " "
                 << mQueExecutor.lastError();
    }
    else
    {
        while (mQueExecutor.next()) {
            resetItemWithResult(item);
            list.push_back(item);
        }
    }
    return list;
}

void OwnDatabase::update(odbitem &item)
{
    mQueExecutor.prepare(SQL_SYNTAX::UPDATE_ITEM_SQL);
    bindValues(item.title, item.author, item.desc, item.label, item.path,
               item.category, item.create_year, item.id);
    if(!mQueExecutor.exec())
    {
        qDebug() << "ERROR: " << SQL_SYNTAX::UPDATE_ITEM_SQL << " "
                 << mQueExecutor.lastError();
    }
}

bool OwnDatabase::remove(odbitem &item)
{
    mQueExecutor.prepare(SQL_SYNTAX::DELETE_ITEM_SQL);
    bindValues(item.id);
    if(!mQueExecutor.exec())
    {
        qDebug() << "ERROR: " << SQL_SYNTAX::DELETE_ITEM_SQL << " "
                 << mQueExecutor.lastError();
        return false;
    }
    return true;
}

int OwnDatabase::categoryCount(int category)
{
    int cnt = 0;
    odbitem item;
    item.category = category;
    mQueExecutor.prepare(SQL_SYNTAX::QUERY_ITEM_CNT_BY_CATEGORY);
    bindValues(item.category);
    if(!mQueExecutor.exec())
    {
        qDebug() << "ERROR: " << SQL_SYNTAX::QUERY_ITEM_CNT_BY_CATEGORY
                 << " " << mQueExecutor.lastError();
    }
    else
    {
        mQueExecutor.next();
        cnt = mQueExecutor.value(0).toInt();
        qDebug() << "DEBUG: COUNT(" << item.category.value() << ") " << cnt;
    }
    return cnt;
}

void OwnDatabase::resetItemWithResult(odbitem &item)
{
    item.id     = mQueExecutor.value(0).toInt();
    item.title  = mQueExecutor.value(1).toString();
    item.author = mQueExecutor.value(2).toString();
    item.desc   = mQueExecutor.value(3).toString();
    item.label  = mQueExecutor.value(4).toInt();
    item.path   = mQueExecutor.value(5).toString();
    item.category = mQueExecutor.value(6).toInt();
    item.create_year = mQueExecutor.value(7).toInt();
    item.init_time = mQueExecutor.value(8).toString();
}

void OwnDatabase::deleteAllStoredItem()
{
    for(auto& shared : mvWaitforRemove)
    {
        if(this->remove(*shared))
        {
            OwnUtil::deleteFile(SQL_TABLE_ITEM::ImgFileLocation() + "/" + shared->path.value());
        }
    }
}

void OwnDatabase::storeDeletingItem(odbitem &item)
{
    auto shared = QSharedPointer<odbitem>(new odbitem(item));
    mvWaitforRemove.push_back(shared);
}

void OwnDatabase::removeDeletingItem(int id)
{
    const int size = mvWaitforRemove.size();
    for (int i = 0; i < size; ++i) {
        if(id == mvWaitforRemove[i]->id.value())
        {
            mvWaitforRemove.remove(i);
            break;
        }
    }
}

QSharedPointer<odbitem> OwnDatabase::returnDeletingItem(int id) const
{
    QSharedPointer<odbitem> shared;
    const int size = mvWaitforRemove.size();
    for (int i = 0; i < size; ++i) {
        shared = mvWaitforRemove[i];
        if(id == shared->id.value())
        {
            return shared;
        }
    }
    shared->id = -1;
    return shared;
}

}
