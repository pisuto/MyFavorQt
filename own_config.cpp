#include "own_config.h"
#include "own_database.h"

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
const QString SQL_TABLE_ITEM::ITEM_CRATEYEAR  = "year";
const QString SQL_TABLE_ITEM::ITEM_INITTIME   = "init_time"; // 获取系统时间


const QString SQL_SYNTAX::CREATE_ITEM_TABLE_SQL =
        "CREATE TABLE IF NOT EXISTS " + SQL_TABLE_ITEM::ITEM_TABLE_NAME + "("  +
        SQL_TABLE_ITEM::ITEM_ID        + " INTEGER PRIMARY KEY AUTOINCREMENT, " +
        SQL_TABLE_ITEM::ITEM_TITLE     + " VARCHAR(40), "        +
        SQL_TABLE_ITEM::ITEM_AUTHOR    + " VARCHAR(30), "        +
        SQL_TABLE_ITEM::ITEM_DESC      + " VARCHAR(100), "       +
        SQL_TABLE_ITEM::ITEM_LABEL     + " INTEGER, "            +
        SQL_TABLE_ITEM::ITEM_IMAGEPATH + " VARCHAR(100), "       +
        SQL_TABLE_ITEM::ITEM_CATEGORY  + " INTEGER, "            +
        SQL_TABLE_ITEM::ITEM_CRATEYEAR + " INTEGER, "            +
        SQL_TABLE_ITEM::ITEM_INITTIME  + " TIMESTAMP NOT NULL DEFAULT (DATETIME('NOW', 'LOCALTIME'))" +
        ")";

const QString SQL_SYNTAX::INSERT_ITEM_SQL =
        "INSERT INTO " + SQL_TABLE_ITEM::ITEM_TABLE_NAME +
        " (" +
        SQL_TABLE_ITEM::ITEM_TITLE     + ", " +
        SQL_TABLE_ITEM::ITEM_AUTHOR    + ", " +
        SQL_TABLE_ITEM::ITEM_DESC      + ", " +
        SQL_TABLE_ITEM::ITEM_LABEL     + ", " +
        SQL_TABLE_ITEM::ITEM_IMAGEPATH + ", " +
        SQL_TABLE_ITEM::ITEM_CATEGORY  + ", " +
        SQL_TABLE_ITEM::ITEM_CRATEYEAR + ") " +
        "VALUES ("   +
        SQL_TABLE_ITEM::Placeholder(SQL_TABLE_ITEM::ITEM_TITLE)  + ", "  +
        SQL_TABLE_ITEM::Placeholder(SQL_TABLE_ITEM::ITEM_AUTHOR) + ", "  +
        SQL_TABLE_ITEM::Placeholder(SQL_TABLE_ITEM::ITEM_DESC)   + ", "  +
        SQL_TABLE_ITEM::Placeholder(SQL_TABLE_ITEM::ITEM_LABEL)  + ", "  +
        SQL_TABLE_ITEM::Placeholder(SQL_TABLE_ITEM::ITEM_IMAGEPATH) + ", " +
        SQL_TABLE_ITEM::Placeholder(SQL_TABLE_ITEM::ITEM_CATEGORY)  + ", " +
        SQL_TABLE_ITEM::Placeholder(SQL_TABLE_ITEM::ITEM_CRATEYEAR) + ")";

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
        SQL_TABLE_ITEM::Placeholder(SQL_TABLE_ITEM::ITEM_CATEGORY)  + ", " +
        SQL_TABLE_ITEM::ITEM_CRATEYEAR  + " = " +
        SQL_TABLE_ITEM::Placeholder(SQL_TABLE_ITEM::ITEM_CRATEYEAR)
        + " WHERE " +
        SQL_TABLE_ITEM::ITEM_ID        + " = " +
        SQL_TABLE_ITEM::Placeholder(SQL_TABLE_ITEM::ITEM_ID);

const QString SQL_SYNTAX::DELETE_ITEM_SQL =
        "DELETE FROM " + SQL_TABLE_ITEM::ITEM_TABLE_NAME + " WHERE " +
        SQL_TABLE_ITEM::ITEM_ID        + " = " +
        SQL_TABLE_ITEM::Placeholder(SQL_TABLE_ITEM::ITEM_ID);

const QString SQL_SYNTAX::QUERY_ITEM_CNT_BY_CATEGORY =
        "SELECT COUNT(" + SQL_TABLE_ITEM::ITEM_ID + ") " +
        "FROM " + SQL_TABLE_ITEM::ITEM_TABLE_NAME + " WHERE " +
        SQL_TABLE_ITEM::ITEM_CATEGORY + " = " +
        SQL_TABLE_ITEM::Placeholder(SQL_TABLE_ITEM::ITEM_CATEGORY);







void OwnConfig::initConfig()
{
    // 在我的文档下面创建文件夹放置图片
    QString location = SQL_TABLE_ITEM::ImgFileLocation();
    QDir dir;
    if(!dir.exists(location) && !dir.mkpath(location))
    {
        qDebug() << "[ERROR] CREATE MYDOCUMENT FILE FAILED.";
    }
    // 计算每一元素的大小
    auto pScreen = QGuiApplication::primaryScreen();
    mElement.setWidth(pScreen->size().width()*1/3*1/4);
    mElement.setHeight(mElement.width()*2);

    // 设置标题font
    QFont font("Microsoft YaHei", 10, QFont::Bold);
    QPalette pale;
    pale.setColor(QPalette::WindowText, Qt::white);

    // 初始化页面配置
    auto categoryCnt = static_cast<int>(SQL_ITEM_CATEGORY::COUNT);
    mPageCount = QVector<int>(categoryCnt - 1, 0);
    for(int i = 1; i < categoryCnt; ++ i)
    {
        updatePages(static_cast<SQL_ITEM_CATEGORY>(i));
    }
}

void OwnConfig::updatePages(SQL_ITEM_CATEGORY category)
{
    auto index = static_cast<int>(category) - 1;
    auto cnt = OwnDatabase::getInstance()->categoryCount(category);
    mPageCount[index] = cnt;
}

}
