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
                                 QString defaultTitle = "title",
                                 QString defaultAuthor = "author",
                                 QString defaultDesc = "desc");

    static void addPages(QStackedWidget* pWidget, int category);

    static void updatePages(QStackedWidget *pWidget, int category, int id, SQL_ITEM_OPER oper);
};

}

#endif // OWN_UTIL_H
