#ifndef OWN_DATABASE_H
#define OWN_DATABASE_H

#include <QSqlDatabase>
#include <QSqlQuery>
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

    void insert(odbitem& item);
    void select(odbitem& item);
    int categoryCount(SQL_ITEM_CATEGORY category);


    template<typename T, typename ...Args>
    void bindValues(T& type, Args&... args)
    {
        mQueExecutor.bindValue(SQL_TABLE_ITEM::Placeholder(type.type_name()),
                               type.value());
        bindValues(args...);
    }

    template<int = 0>
    void bindValues() {}

public:
    QSqlDatabase mDBAllocator;
    QSqlQuery mQueExecutor;
};

}

#endif // OWN_DATABASE_H
