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

#if 0 /* 创建默认模板 */
    ref::format_helper helper(new ref::xml_parser("default.xml"));
    oconfig config;
    config.trayed = false;
    config.category = { {{"Anime", ":/images/svgtopng/anime.png"}, {"Manga", ":/images/svgtopng/manga.png"},
                         {"Movie", ":/images/svgtopng/movie.png"}, {"Music", ":/images/svgtopng/music.png"},
                         {"Photo", ":/images/svgtopng/photos.png"}} };
    config.screen = { 0, {3, 3}, {{{2560, 1440},{185, 300}}, {{1920, 1080},{138, 225}}} };
    config.btngrp = { -1, 40, {{-1, {40, 30}, {30, 30}}, {0, {80, 30}, {20, 20}}, {1, {80, 30}, {20, 20}},
                    {2, {80, 30}, {20, 20}}, {3, {80, 30}, {20, 20}}, {4, {80, 30}, {20, 20}},
                    {5, {40, 30}, {20, 20}}} };
    config.element = { {{"title", 15, 75, "Microsoft YaHei", ""}, {"user", 10, 50, "Microsoft YaHei", ""},
                        {"desc", 10, 50, "Microsoft YaHei", ""}} };
    config.pagebar = { {{"text", 9, 0, "Microsoft YaHei", ""}} };

    helper.write(config);
#endif

    // 结束时保存配置
    spConfig->save();
    return a.exec();
}
