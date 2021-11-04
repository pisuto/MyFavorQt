#include "own_database.h"
#include "own_database_item.h"

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

void OwnDatabase::insert(odbitem &item)
{
    mQueExecutor.prepare(SQL_SYNTAX::INSERT_ITEM_SQL);
    bindValues(item.title, item.author, item.desc, item.label, item.path, item.category);
    if(!mQueExecutor.exec())
    {
        qDebug() << "ERROR" << SQL_SYNTAX::INSERT_ITEM_SQL << " "
                 << mQueExecutor.lastError();
    }
}

void OwnDatabase::select(odbitem &item)
{
    mQueExecutor.prepare(SQL_SYNTAX::SELECT_ITEM_SQL);
    bindValues(item.id);
    if(!mQueExecutor.exec())
    {
        qDebug() << "ERROR" << SQL_SYNTAX::SELECT_ITEM_SQL << " "
                 << mQueExecutor.lastError();
    }
    else
    {
        while(mQueExecutor.next())
        {
            item.id     = mQueExecutor.value(0).toInt();
            item.title  = mQueExecutor.value(1).toString();
            item.author = mQueExecutor.value(2).toString();
            item.desc   = mQueExecutor.value(3).toString();
            item.label  = mQueExecutor.value(4).toInt();
            item.path   = mQueExecutor.value(5).toString();
            item.category = mQueExecutor.value(6).toInt();
            qDebug() << item;
        }
    }
}





}
