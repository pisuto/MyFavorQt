#ifndef OWN_UTIL_H
#define OWN_UTIL_H

#include <QtMath>
#include <QString>

#include "own_config.h"

class QFrame;
class QStackedWidget;

namespace mf {

class OwnUtil {
public:
    static float triangleFunc(float x);

    static bool deleteFile(const QString& path);

    static QImage getImgWithOverlay(const QImage& base, const QImage& overlay);

    static int getPages(int category);

    static QFrame *createNewPage(int start, int category,
                                 QString defaultPath = ":/images/svgtopng/default.png",
                                 QString defaultTitle = "",
                                 QString defaultAuthor = "",
                                 QString defaultDesc = "");

    static void addPages(QStackedWidget* pWidget, int category);

    static void updatePages(QStackedWidget *pWidget, int category, int id, SQL_ITEM_OPER oper);

    static QString strAutoFeed(const QString& text, const QFont& font, int row = 2, int px = 40);
};

}

#endif // OWN_UTIL_H
