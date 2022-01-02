#-------------------------------------------------
#
# Project created by QtCreator 2021-10-18T19:10:37
#
#-------------------------------------------------

QT       += core gui sql xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

RC_FILE = resource/icon.rc
TARGET = MyFavor
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

SOURCES += \
        components/dialog/own_imageviewer.cpp \
        components/dialog/own_itemuploadform.cpp \
        components/dialog/own_screencapture.cpp \
        components/dialog/own_itemuploadview.cpp \
        MainWindow.cpp \
        main.cpp \
        components/pagebar/own_pagebar.cpp \
        components/edit/own_lineedit.cpp \
        components/button/own_button.cpp \
        components/button/own_multilabels.cpp \
        components/global/own_config.cpp \
        components/global/own_database.cpp \
        components/global/own_util.cpp \
        components/viewer/own_element.cpp \
        components/viewer/own_fadestackedview.cpp \
        components/viewer/own_mainwidget.cpp \
        components/reflection/own_reflect_item.cpp \
        components/reflection/reflection/ref_define.cpp \
        components/reflection/tool/ref_parser.cpp \
        components/viewer/own_aboutview.cpp \
        components/viewer/own_addlabelview.cpp \
        components/viewer/own_settingview.cpp \
        components/viewer/own_slidestackedview.cpp \

HEADERS += \
        components/dialog/own_imageviewer.h \
        components/dialog/own_itemuploadform.h \
        components/dialog/own_screencapture.h \
        components/dialog/own_itemuploadview.h \
        MainWindow.h \
        components/pagebar/own_pagebar.h \
        components/edit/own_lineedit.h \
        components/button/own_button.h \
        components/button/own_multilabels.h \
        components/global/own_config.h \
        components/global/own_database.h \
        components/global/own_util.h \
        components/global/own_database_item.h \
        components/viewer/own_element.h \
        components/viewer/own_fadestackedview.h \
        components/viewer/own_mainwidget.h \
        components/reflection/own_reflect_item.h \
        components/reflection/reflection/ref_define.h \
        components/reflection/tool/ref_parser.h \
        components/reflection/reflection/ref_base.h \
        components/reflection/reflection/ref_traits.h \
        components/reflection/tool/ref_helper.h \
        components/viewer/own_aboutview.h \
        components/viewer/own_addlabelview.h \
        components/viewer/own_settingview.h \
        components/viewer/own_slidestackedview.h \


FORMS += \
        ui/MainWindow.ui \
        ui/OwnItemUploadView.ui \
        ui/OwnAddLabelView.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resource/images.qrc \
    resource/qss.qrc

