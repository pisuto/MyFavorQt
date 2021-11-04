#include "MainWindow.h"
#include "own_config.h"

#include <QApplication>
#include <QFile>

#include "own_database.h"
#include "own_database_item.h"

int main(int argc, char *argv[])
{
    // 不清晰解决
    QGuiApplication::setAttribute(Qt::AA_EnableHighDpiScaling );
    QGuiApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);
    QApplication a(argc, argv);
    // 加载qss
    QString qss;
    QFile qssFile(":/style/qss.qss");
    qssFile.open(QFile::ReadOnly);
    if(qssFile.isOpen())
    {
        qss = QLatin1String(qssFile.readAll());
        qApp->setStyleSheet(qss);
        qssFile.close();
    }
    // 初始化配置
    auto spConfig = mf::OwnConfig::getInstance();
    spConfig->initConfig();


    // for test
    mf::odbitem item;
    item.id.value(1);
    item.title.value("hello world");
    mf::OwnDatabase db;
    db.insert(item);
    db.select(item);

    MainWindow w;
    w.show();

    return a.exec();
}
