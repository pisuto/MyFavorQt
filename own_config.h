#ifndef OWN_CONFIG_H
#define OWN_CONFIG_H

#include <QWidget>
#include <QMutex>
#include <QSharedPointer>
#include <QScreen>
#include <QGuiApplication>
#include <QLabel>
#include <QStandardPaths>
#include <QDir>
#include <QDebug>

namespace mf {

// 对元素的操作类型
enum class SQL_ITEM_OPER
{
    NONE,
    INSERT,
    UPDATE,
    DELETE,
};

// 展示的图片大小
enum class IMAGE_DISPLAY_SIZE
{
    WIDTH = 185,
    HEIGHT = 300,
};

// 标签类型
enum class SQL_ITEM_CATEGORY
{
    EMPTY = 0,
    ANIME = 1,
    MANGA = 2,
    MOVIE = 3,
    MUSIC = 4,
    PHOTO = 5,
    COUNT = 5,
};

enum class SQL_PAGE_ITEM_GRID
{
    ROW   = 3,
    COL   = 3,
    COUNT = ROW * COL,
};

// sql相关全局数据
struct SQL_TABLE_ITEM
{
    static const QString ITEM_DATABASE_NAME;
    static const QString ITEM_TABLE_NAME;
    static const QString ITEM_ID;
    static const QString ITEM_TITLE;
    static const QString ITEM_AUTHOR;
    static const QString ITEM_DESC;
    static const QString ITEM_LABEL;
    static const QString ITEM_IMAGEPATH;
    static const QString ITEM_CATEGORY;
    static const QString ITEM_CRATEYEAR;
    static const QString ITEM_INITTIME;

    static QString ImgFileLocation() { return QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation) + "/MyFavor"; }
    static QString Placeholder(const QString& item) { return ":" + item; }
    static QString CategoryToString(SQL_ITEM_CATEGORY index)  {
        switch (index)
        {
        case SQL_ITEM_CATEGORY::ANIME:
            return "Anime";
        case SQL_ITEM_CATEGORY::MANGA:
            return  "Manga";
        case SQL_ITEM_CATEGORY::MOVIE:
            return "Movie";
        case SQL_ITEM_CATEGORY::MUSIC:
            return "Music";
        case SQL_ITEM_CATEGORY::PHOTO:
            return "Photo";
        default:
            return "Null";

        }
    }
};

struct SQL_SYNTAX
{
    static const QString CREATE_ITEM_TABLE_SQL;
    static const QString INSERT_ITEM_SQL;
    static const QString SELECT_ITEM_SQL;
    static const QString UPDATE_ITEM_SQL;
    static const QString DELETE_ITEM_SQL;
    static const QString QUERY_ITEM_CNT_BY_CATEGORY;
    /* 需要输入开始获取的位置参数 */
    static const QString SELECT_ITEM_POS_OFFSET_SQL;
};

// 单例模式基类
template<typename T>
class OwnSingleton
{
public:
    static QSharedPointer<T> getInstance()
    {
        if(mspInstance.isNull())
        {
            QMutexLocker locker(&mMutex);
            mspInstance.reset(new T);
        }
        return mspInstance;
    }

    OwnSingleton(const OwnSingleton&) = delete ;
    OwnSingleton(OwnSingleton&&) = delete ;
    OwnSingleton& operator=(const OwnSingleton&) = delete ;

protected:
    OwnSingleton() {}
    virtual ~OwnSingleton() {}

private:
    static QMutex mMutex;
    static QSharedPointer<T> mspInstance;
};

template<typename T>
QSharedPointer<T> OwnSingleton<T>::mspInstance;

template<typename T>
QMutex OwnSingleton<T>::mMutex;

class OwnItemUploadView;

// 单例配置类
class OwnConfig : public OwnSingleton<OwnConfig>
{
public:
    OwnConfig() : mpMainWindow(Q_NULLPTR), mpItemViewer(Q_NULLPTR) {}
    ~OwnConfig() {}

    void initConfig();

    void setFont(QLabel* pLabel, int size = 7, QFont::Weight weight = QFont::Normal)
    {
        QFont font("Microsoft YaHei", size, weight);
        pLabel->setFont(font);
    }

    const QVector<int>& getCategoryCount() const { return mPageCount; }
    void updateCategoryCount(SQL_ITEM_CATEGORY category);

    QSize getDisplayImageSize() const { return QSize(static_cast<int>(IMAGE_DISPLAY_SIZE::WIDTH), static_cast<int>(IMAGE_DISPLAY_SIZE::HEIGHT)); }

    QWidget* getMainWindowPtr() const { return mpMainWindow; }
    void setMainWindowPtr(QWidget* pointer) { mpMainWindow = pointer; }

    void setTrayed(bool result) { mbTrayed = result; }
    bool getTrayed() const { return mbTrayed; }
    void hideWindowToTray();
    void showWindowFromTray();

    OwnItemUploadView *getItemViewer();

private:
    QVector<int> mPageCount;
    QWidget* mpMainWindow;
    mf::OwnItemUploadView* mpItemViewer;
    bool mbTrayed;
};

}

#endif // OWN_CONFIG_H
