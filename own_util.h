#ifndef OWN_UTIL_H
#define OWN_UTIL_H

#include <QtMath>
#include <QString>

#include "own_config.h"

class QFrame;
class QStackedWidget;

namespace mf {

class odbitem;

class OwnUtil {
public:
    static float triangleFunc(float x);

    static void createFile(const QString& fileName);

    static bool deleteFile(const QString& path);

    static QImage getImgWithOverlay(const QImage& base, const QImage& overlay);

    /* category从1开始 */
    static int getPages(int category);

    static QFrame *createNewPage(int start, int category,
                                 QString defaultPath = ":/images/svgtopng/default.png",
                                 QString defaultTitle = "",
                                 QString defaultAuthor = "",
                                 QString defaultDesc = "");

    static void addPages(QStackedWidget* pWidget, int category);

    static void updatePages(QStackedWidget *pWidget, odbitem& item, int category, int id, SQL_ITEM_OPER oper);

    static QString strAutoFeed(const QString& text, const QFont& font, int row = 2, int px = 40);

    static QString QRgbaToQString(QRgb color) { return QString("rgba(%1, %2, %3, %4)").arg(qRed(color)).arg(qGreen(color)).arg(qBlue(color)).arg(qAlpha(color)); }
};

}

#endif // OWN_UTIL_H
