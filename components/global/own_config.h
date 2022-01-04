/**
* This file is part of MyFavor
*
* Copyright (C) 2021-2022 Pisuto.
*
*/

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

#include "../reflection/own_reflect_item.h"

#define MF_DEFAULT_XML  "default.xml"
#define MF_CUSTOM_XML   "setting.xml"

class MainWindow;

namespace mf {

// 对元素的操作类型
enum class SQL_ITEM_OPER
{
    NONE,
    INSERT,
    UPDATE,
    DELETE,
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
class OwnMultiLabels;
class OwnSystemView;
class OwnMainWidget;
class OwnToggleButton;

enum  HANDLER_OPER{
    OPER_NULL = 0,
    OPER_DEL = 1,
    OPER_ADD,
    OPER_MOD,
    OPER_GET,
    OPER_MIN,
    OPER_HIDE,
    OPER_SHOW,
};

// 单例配置类
class OwnConfig : public OwnSingleton<OwnConfig>
{
public:
    OwnConfig() : mpMainWindow(Q_NULLPTR), mpItemViewer(Q_NULLPTR),
        helper(new ref::xml_parser(MF_CUSTOM_XML)) {}
    ~OwnConfig() {}

    // 待整改
    QSize getDisplayImageSize() const {
        if(data.screen.index >= data.screen.resogrp.size()) return QSize(0, 0);
        const auto& image = data.screen.resogrp[data.screen.index].image;
        return QSize(static_cast<int>(image.width), static_cast<int>(image.height));
    }

    oconfig& getSettingData() { return data; }
    const QVector<int>& getCategoryCount() const { return mPageCount; }
    void updateCategoryCount(int category);
    OwnItemUploadView* getItemViewer() const { return mpItemViewer; }
    OwnToggleButton* getToggleButton() const { return mpToggle; }

    // 处理函数
    template<typename T, typename... Args>
    void handler(T*, int /* HANDLER_OPER */, Args...) {}

    template<typename... Args>
    void handler() {
        resolve_handler();
    }

    template<>
    void handler(MainWindow* object, int oper) {
        resolve_handler(object, oper);
    }

    template<>
    void handler(OwnMainWidget* object, int oper) {
        resolve_handler(object, oper);
    }

    template<>
    void handler(OwnItemUploadView* object, int oper) {
        resolve_handler(object, oper);
    }

    template<>
    void handler(OwnToggleButton* object, int oper) {
        resolve_handler(object, oper);
    }

    template<>
    void handler(OwnMultiLabels* object, int oper, const QString& name, const QString& path) {
        resolve_handler(object, oper, name, path);
    }

    template<>
    void handler(OwnMultiLabels* object, int oper, int index) {
        resolve_handler(object, oper, index);
    }

    template<>
    void handler(OwnSystemView* object, int oper, QColor color) {
        resolve_handler(object, oper, color);
    }

    template<>
    void handler(OwnSystemView* object, int oper, bool apply) {
        resolve_handler(object, oper, apply);
    }

private:
    // 处理函数专用函数
    void resolve_handler();

    void resolve_handler(MainWindow* object, int oper);
    void resolve_handler(OwnMainWidget* object, int oper);
    void resolve_handler(OwnItemUploadView* object, int oper);
    void resolve_handler(OwnToggleButton* object, int oper);

    void resolve_handler(OwnMultiLabels* object, int oper, const QString& name, const QString& path);
    void resolve_handler(OwnMultiLabels* object, int oper, int index);

    void resolve_handler(OwnSystemView* object, int oper, QColor color);
    void resolve_handler(OwnSystemView* object, int oper, bool apply);

private:
    QVector<int> mPageCount;
    QWidget* mpMainWindow;
    QWidget* mpMainWidget;
    mf::OwnItemUploadView* mpItemViewer;
    mf::OwnToggleButton* mpToggle;

    /* 全局配置数据 */
    oconfig data;
    ref::format_helper helper;
};

}

#endif // OWN_CONFIG_H
