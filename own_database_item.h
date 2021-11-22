#ifndef OWN_DATABASE_ITEM_H
#define OWN_DATABASE_ITEM_H

#include <QString>
#include <QDebug>

#include "own_database.h"

namespace mf {

template <typename T>
class oobject
{
public:
    using value_type = T;
    using reference_type = T&;
    using const_reference_type = const T&;

public:
    explicit oobject(value_type val, QString name) : content(val), typeName(name) {}
    explicit oobject(QString name) : typeName(name) {}
    explicit oobject(const oobject& rhs) : oobject(rhs.content, rhs.typeName) {}

    void value(value_type val) { content = val; }
    value_type value() const { return content; }
    QString type_name() const { return typeName; }

    oobject& operator= (const_reference_type rhs) { value(rhs); return *this; }

private:
    value_type content;
    QString typeName;
};

class odbitem
{
public:
    odbitem() :
        id(SQL_TABLE_ITEM::ITEM_ID),
        title(SQL_TABLE_ITEM::ITEM_TITLE),
        author(SQL_TABLE_ITEM::ITEM_AUTHOR),
        desc(SQL_TABLE_ITEM::ITEM_DESC),
        label(SQL_TABLE_ITEM::ITEM_LABEL),
        path(SQL_TABLE_ITEM::ITEM_IMAGEPATH),
        category(SQL_TABLE_ITEM::ITEM_CATEGORY),
        create_year(SQL_TABLE_ITEM::ITEM_CRATEYEAR),
        init_time(SQL_TABLE_ITEM::ITEM_INITTIME)
    {}

    odbitem(const odbitem& rhs) :
        id(rhs.id),
        title(rhs.title),
        author(rhs.author),
        desc(rhs.desc),
        label(rhs.label),
        path(rhs.path),
        category(rhs.category),
        create_year(rhs.create_year),
        init_time(rhs.init_time)
    {}


    oobject<int>     id;
    oobject<QString> title;
    oobject<QString> author;
    oobject<QString> desc;
    oobject<int>     label;
    oobject<QString> path;
    oobject<int>     category;
    oobject<int>     create_year;
    oobject<QString> init_time;

    friend QDebug operator<< (QDebug out, const odbitem& item)
    {
        out << item.id.type_name() << ": " << item.id.value() << "\n"
            << item.title.type_name() << ": " << item.title.value() << "\n"
            << item.author.type_name() << ": " << item.author.value() << "\n"
            << item.desc.type_name() << ": " << item.desc.value() << "\n"
            << item.label.type_name() << ": " << item.label.value() << "\n"
            << item.path.type_name() << ": " << item.path.value() << "\n"
            << item.category.type_name() << ": " << item.category.value() << "\n"
            << item.create_year.type_name() << ": " << item.create_year.value() << "\n"
            << item.init_time.type_name() << ": " << item.init_time.value() << "\n";
        return out;
    }
};

}

#endif // OWN_DATABASE_ITEM_H
