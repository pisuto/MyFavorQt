#include "own_config.h"
#include "own_database.h"
#include "own_util.h"
#include "item_upload_window/own_itemuploadview.h"

#include "MainWindow.h"

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
        SQL_TABLE_ITEM::ITEM_ID                          + " = " +
        SQL_TABLE_ITEM::Placeholder(SQL_TABLE_ITEM::ITEM_ID);

const QString SQL_SYNTAX::QUERY_ITEM_CNT_BY_CATEGORY =
        "SELECT COUNT(" + SQL_TABLE_ITEM::ITEM_ID + ") " +
        "FROM " + SQL_TABLE_ITEM::ITEM_TABLE_NAME + " WHERE " +
        SQL_TABLE_ITEM::ITEM_CATEGORY             + " = " +
        SQL_TABLE_ITEM::Placeholder(SQL_TABLE_ITEM::ITEM_CATEGORY);

const QString SQL_SYNTAX::SELECT_ITEM_POS_OFFSET_SQL =
        "SELECT * FROM " + SQL_TABLE_ITEM::ITEM_TABLE_NAME + " WHERE " +
        SQL_TABLE_ITEM::ITEM_CATEGORY                      + " = " +
        SQL_TABLE_ITEM::Placeholder(SQL_TABLE_ITEM::ITEM_CATEGORY) +
        " ORDER BY " + SQL_TABLE_ITEM::ITEM_ID             + " ASC" +
        " LIMIT %1 OFFSET %2";


/*--------------------------------------------------------------------------*/


void OwnConfig::updateCategoryCount(int category)
{
    auto cnt = OwnDatabase::getInstance()->categoryCount(category);
    mPageCount[category - 1] = cnt;
}


/*-------------------------------------------------------------------------------------------------------------------*/


void OwnConfig::resolve_handler()
{
    if(!mf::OwnUtil::checkXmlExists()) {
        return;
    }

    // 在我的文档下面创建文件夹放置图片
    QString location = SQL_TABLE_ITEM::ImgFileLocation();
    QDir dir;
    if(!dir.exists(location) && !dir.mkpath(location))
    {
        qDebug() << "[ERROR] CREATE MYDOCUMENT FILE FAILED.";
    }

    // 初始化全局数据
    helper.read(data);

    // 初始化页面配置
    auto categoryCnt = static_cast<int>(data.category.categories.size());
    mPageCount = QVector<int>(categoryCnt, 0);
    for(int i = 1; i <= categoryCnt; ++ i)
    {
        updateCategoryCount(i);
    }
}

void OwnConfig::resolve_handler(MainWindow *object, int oper)
{
    switch (oper) {
    case HANDLER_OPER::OPER_ADD:
    {
        Q_ASSERT(object);
        mpMainWindow = object;
        break;
    }
    case HANDLER_OPER::OPER_MIN:
    {
        auto window = qobject_cast<MainWindow*>(mpMainWindow);
        window->showMinimized();
        break;
    }
    case HANDLER_OPER::OPER_HIDE:
    {
        auto window = qobject_cast<MainWindow*>(mpMainWindow);
        window->hide();
        break;
    }
    case HANDLER_OPER::OPER_SHOW:
    {
        auto window = qobject_cast<MainWindow*>(mpMainWindow);
        window->showNormal();
        break;
    }
    default:
        break;
    }
}

void OwnConfig::resolve_handler(OwnMainWidget *object, int oper)
{
    Q_ASSERT(object);
    if(oper == HANDLER_OPER::OPER_ADD) {
        mpMainWidget = object;
    }
    else if(oper == HANDLER_OPER::OPER_DEL) {
        /* 析构 */
        helper.write(data);
    }
}

void OwnConfig::resolve_handler(OwnItemUploadView *object, int oper)
{
    switch (oper) {
    case HANDLER_OPER::OPER_ADD:
    {
        Q_ASSERT(object);
        mpItemViewer = object;
        break;
    }
    case HANDLER_OPER::OPER_GET:
    {
        object = mpItemViewer;
        break;
    }
    default:
        break;
    }
}

void OwnConfig::resolve_handler(OwnToggleButton* object, int oper)
{
    switch (oper) {
    case HANDLER_OPER::OPER_ADD:
    {
        Q_ASSERT(object);
        mpToggle = object;
        break;
    }
    case HANDLER_OPER::OPER_GET:
    {
        object = mpToggle;
        break;
    }
    default:
        break;
    }
}

void OwnConfig::resolve_handler(OwnMultiLabels *object, int oper, const QString &name, const QString &path)
{
    Q_UNUSED(object);
    if(oper == HANDLER_OPER::OPER_ADD) {
        data.category.categories.push_back({name.toStdString(), path.toStdString()});
    }
}

void OwnConfig::resolve_handler(OwnMultiLabels *object, int oper, int index)
{
    Q_UNUSED(object);
    if(oper == HANDLER_OPER::OPER_DEL) {
        auto& categories = data.category.categories;
        const auto count = categories.size();
        for(size_t i = static_cast<size_t>(index); i < count - 1; ++i) {
            categories[i] = categories[i + 1];
        }
        categories.pop_back();
    }
}

void OwnConfig::resolve_handler(OwnSystemView *object, int oper, QColor color)
{
    Q_UNUSED(object);
    Q_ASSERT(mpMainWidget);
    if(oper == HANDLER_OPER::OPER_MOD) {
        int red = 255, green = 255, blue = 255;
        color.getRgb(&red, &green, &blue);
        auto pWidget = qobject_cast<OwnMainWidget*>(mpMainWidget);
        pWidget->setStyleSheet(QString("QWidget#own_mainwidget{background-color:rgb(%1, %2, %3);}"
                                       "QFrame#own_fadeview{background-color:rgb(%1, %2, %3);}")
                            .arg(red).arg(green).arg(blue));
        data.bgcolor = {red, green, blue, 255};
    }
}

void OwnConfig::resolve_handler(OwnSystemView *object, int oper, bool apply)
{
    Q_UNUSED(object);
    Q_UNUSED(object);
    Q_UNUSED(apply);
    switch (oper) {
    case HANDLER_OPER::OPER_MOD:
    {
        Q_ASSERT(mpMainWidget);
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(mpMainWidget, "Delete", "Please confirm whether to overwrite setting file?", QMessageBox::Yes | QMessageBox::No);
        if(reply == QMessageBox::Yes)
        {
            if(OwnUtil::copyFile("default.xml", "setting.xml"))
            {
            }
        }
        break;
    }
    default:
        break;
    }
}

}
