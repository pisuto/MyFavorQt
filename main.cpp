#include "MainWindow.h"
#include "own_config.h"

#include <QApplication>
#include <QFile>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QString qss;
    QFile qssFile(":/style/qss.qss");
    qssFile.open(QFile::ReadOnly);
    if(qssFile.isOpen())
    {
        qss = QLatin1String(qssFile.readAll());
        qApp->setStyleSheet(qss);
        qssFile.close();
    }

    auto spConfig = mf::OwnConfig::getInstance();
    spConfig->initConfig();

    MainWindow w;
    w.show();

    return a.exec();
}
