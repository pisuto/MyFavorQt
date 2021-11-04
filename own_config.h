#ifndef OWN_CONFIG_H
#define OWN_CONFIG_H

#include <QWidget>
#include <QMutex>
#include <QSharedPointer>
#include <QScreen>
#include <QGuiApplication>
#include <QLabel>

namespace mf {

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

// 单例配置类
class OwnConfig : public OwnSingleton<OwnConfig>
{
public:
    OwnConfig() {}
    ~OwnConfig() {}

    void initConfig()
    {
        // 计算每一元素的大小
        auto pScreen = QGuiApplication::primaryScreen();
        mElement.setWidth(pScreen->size().width()*1/3*1/4);
        mElement.setHeight(mElement.width()*2);

        // 设置标题font
        QFont font("Microsoft YaHei", 10, QFont::Bold);
        QPalette pale;
        pale.setColor(QPalette::WindowText, Qt::white);
    }

    QSize getElementSize() const { return mElement; }

    void setTitleStyle(QLabel* pTitle)
    {
        QFont font("Microsoft YaHei", 15, QFont::Bold);
        QPalette pale;
        pale.setColor(QPalette::WindowText, Qt::white);
        pTitle->setFont(font);
        pTitle->setPalette(pale);
    }

    void setFont(QLabel* pLabel, int size = 7, QFont::Weight weight = QFont::Normal)
    {
        QFont font("Microsoft YaHei", size, weight);
        pLabel->setFont(font);
    }

private:
    QSize mElement;

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

    static QString Placeholder(const QString& item) { return ":" + item; }
};

struct SQL_SYNTAX
{
    static const QString CREATE_ITEM_TABLE_SQL;
    static const QString INSERT_ITEM_SQL;
    static const QString SELECT_ITEM_SQL;
    static const QString UPDATE_ITEM_SQL;
    static const QString DELETE_ITEM_SQL;
};

}

#endif // OWN_CONFIG_H
