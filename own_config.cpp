#include "own_config.h"

namespace mf {

const QString SQL_TABLE_ITEM::ITEM_DATABASE_NAME = "myfavor.db";
const QString SQL_TABLE_ITEM::ITEM_TABLE_NAME = "myfavoritems";
const QString SQL_TABLE_ITEM::ITEM_ID         = "id";
const QString SQL_TABLE_ITEM::ITEM_TITLE      = "title";
const QString SQL_TABLE_ITEM::ITEM_AUTHOR     = "author";
const QString SQL_TABLE_ITEM::ITEM_DESC       = "description";
const QString SQL_TABLE_ITEM::ITEM_LABEL      = "label";
const QString SQL_TABLE_ITEM::ITEM_IMAGEPATH  = "path";
const QString SQL_TABLE_ITEM::ITEM_CATEGORY   = "category";


const QString SQL_SYNTAX::CREATE_ITEM_TABLE_SQL =
        "CREATE TABLE IF NOT EXISTS " + SQL_TABLE_ITEM::ITEM_TABLE_NAME + "("  +
        SQL_TABLE_ITEM::ITEM_ID        + " INTEGER PRIMARY KEY AUTOINCREMENT, " +
        SQL_TABLE_ITEM::ITEM_TITLE     + " VARCHAR(40), "        +
        SQL_TABLE_ITEM::ITEM_AUTHOR    + " VARCHAR(30), "        +
        SQL_TABLE_ITEM::ITEM_DESC      + " VARCHAR(100), "       +
        SQL_TABLE_ITEM::ITEM_LABEL     + " INTEGER, "             +
        SQL_TABLE_ITEM::ITEM_IMAGEPATH + " VARCHAR(100), "       +
        SQL_TABLE_ITEM::ITEM_CATEGORY  + " INTEGER)";

const QString SQL_SYNTAX::INSERT_ITEM_SQL =
        "INSERT INTO " + SQL_TABLE_ITEM::ITEM_TABLE_NAME +
        " (" +
        SQL_TABLE_ITEM::ITEM_TITLE     + ", " +
        SQL_TABLE_ITEM::ITEM_AUTHOR    + ", " +
        SQL_TABLE_ITEM::ITEM_DESC      + ", " +
        SQL_TABLE_ITEM::ITEM_LABEL     + ", " +
        SQL_TABLE_ITEM::ITEM_IMAGEPATH + ", " +
        SQL_TABLE_ITEM::ITEM_CATEGORY  + ")" +
        " VALUES ("   +
        SQL_TABLE_ITEM::Placeholder(SQL_TABLE_ITEM::ITEM_TITLE)  + ", "  +
        SQL_TABLE_ITEM::Placeholder(SQL_TABLE_ITEM::ITEM_AUTHOR) + ", "  +
        SQL_TABLE_ITEM::Placeholder(SQL_TABLE_ITEM::ITEM_DESC)   + ", "  +
        SQL_TABLE_ITEM::Placeholder(SQL_TABLE_ITEM::ITEM_LABEL)  + ", "  +
        SQL_TABLE_ITEM::Placeholder(SQL_TABLE_ITEM::ITEM_IMAGEPATH) + ", " +
        SQL_TABLE_ITEM::Placeholder(SQL_TABLE_ITEM::ITEM_CATEGORY)  + ")";

const QString SQL_SYNTAX::SELECT_ITEM_SQL =
        "SELECT * FROM " + SQL_TABLE_ITEM::ITEM_TABLE_NAME + " WHERE " +
        SQL_TABLE_ITEM::ITEM_ID + " = " +
        SQL_TABLE_ITEM::Placeholder(SQL_TABLE_ITEM::ITEM_ID);

const QString SQL_SYNTAX::UPDATE_ITEM_SQL =
        "UPDATE " + SQL_TABLE_ITEM::ITEM_TABLE_NAME + " SET " +
        SQL_TABLE_ITEM::ITEM_TITLE     + " = " +
        SQL_TABLE_ITEM::Placeholder(SQL_TABLE_ITEM::ITEM_TITLE)  + ", "  +
        SQL_TABLE_ITEM::ITEM_AUTHOR    + " = " +
        SQL_TABLE_ITEM::Placeholder(SQL_TABLE_ITEM::ITEM_AUTHOR) + ", "  +
        SQL_TABLE_ITEM::ITEM_DESC      + " = " +
        SQL_TABLE_ITEM::Placeholder(SQL_TABLE_ITEM::ITEM_DESC)   + ", "  +
        SQL_TABLE_ITEM::ITEM_LABEL     + " = " +
        SQL_TABLE_ITEM::Placeholder(SQL_TABLE_ITEM::ITEM_LABEL)  + ", "  +
        SQL_TABLE_ITEM::ITEM_IMAGEPATH + " = " +
        SQL_TABLE_ITEM::Placeholder(SQL_TABLE_ITEM::ITEM_IMAGEPATH) + ", " +
        SQL_TABLE_ITEM::ITEM_CATEGORY  + " = " +
        SQL_TABLE_ITEM::Placeholder(SQL_TABLE_ITEM::ITEM_CATEGORY)  + " WHERE " +
        SQL_TABLE_ITEM::ITEM_ID        + " = " +
        SQL_TABLE_ITEM::Placeholder(SQL_TABLE_ITEM::ITEM_ID);

const QString SQL_SYNTAX::DELETE_ITEM_SQL =
        "DELETE FROM " + SQL_TABLE_ITEM::ITEM_TABLE_NAME + " WHERE " +
        SQL_TABLE_ITEM::ITEM_ID        + " = " +
        SQL_TABLE_ITEM::Placeholder(SQL_TABLE_ITEM::ITEM_ID);

}
