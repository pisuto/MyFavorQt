#ifndef OWN_DATABASE_H
#define OWN_DATABASE_H

#include <QSqlDatabase>
#include <QSqlQuery>
/**
* This file is part of MyFavor
*
* Copyright (C) 2021-2022 Pisuto.
*
*/

#include <QSqlError>

#include "own_config.h"

namespace mf {
    class odbitem;
}

namespace mf {

class OwnDatabase : public OwnSingleton<OwnDatabase>
{
public:
    OwnDatabase();
    ~OwnDatabase() { mDBAllocator.close(); }

    int insert(odbitem& item);
    odbitem select(odbitem& item);
    void update(odbitem& item);
    bool remove(odbitem& item);
    int categoryCount(int category);

    /* 输入的pos为一页的起始 */
    QList<odbitem> selectOnePage(int pos, int count, int category);
    /* 保存即将删除的元素，在退出时全部删除 */
    void storeDeletingItem(odbitem& item);
    void removeDeletingItem(int id);
    QSharedPointer<odbitem> returnDeletingItem(int id) const;
    void deleteAllStoredItem();

private:

    template<typename ...Args>
    void bindValues(Args&...) {}

    template<>
    void bindValues() {}

    template<typename T, typename ...Args>
    void bindValues(T& type, Args&... args)
    {
        mQueExecutor.bindValue(SQL_TABLE_ITEM::Placeholder(type.type_name()),
                               type.value());
        bindValues(args...);
    }

    void resetItemWithResult(odbitem& item);

private:
    QSqlDatabase mDBAllocator;
    QSqlQuery mQueExecutor;
    QVector<QSharedPointer<odbitem>> mvWaitforRemove;
};

}

#endif // OWN_DATABASE_H
