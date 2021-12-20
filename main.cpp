#include "MainWindow.h"
#include "own_config.h"

#include <QApplication>
#include <QFile>

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
    spConfig->init();

    MainWindow w;
    spConfig->setMainWindowPtr(&w);
    w.show();

    // 结束时保存配置
    spConfig->save();
    return a.exec();
}
