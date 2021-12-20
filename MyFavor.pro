#-------------------------------------------------
#
# Project created by QtCreator 2021-10-18T19:10:37
#
#-------------------------------------------------

QT       += core gui sql xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

RC_FILE = icon.rc
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
    item_upload_window/own_imageviewer.cpp \
    item_upload_window/own_itemuploadform.cpp \
    item_upload_window/own_screencapture.cpp \
        main.cpp \
        MainWindow.cpp \
        own_button.cpp \
        own_config.cpp \
        own_database.cpp \
        own_element.cpp \
        own_fadestackedview.cpp \
        own_lineedit.cpp \
        own_mainwidget.cpp \
        own_pagebar.cpp \
    own_reflection/own_reflect_item.cpp \
    own_reflection/reflection/ref_define.cpp \
    own_reflection/tool/ref_parser.cpp \
        own_slidestackedview.cpp \
        item_upload_window/own_itemuploadview.cpp \
        own_util.cpp

HEADERS += \
        MainWindow.h \
        item_upload_window/own_imageviewer.h \
        item_upload_window/own_itemuploadform.h \
        item_upload_window/own_itemuploadview.h \
        item_upload_window/own_screencapture.h \
        own_button.h \
        own_config.h \
        own_database.h \
        own_database_item.h \
        own_element.h \
        own_fadestackedview.h \
        own_lineedit.h \
        own_mainwidget.h \
        own_pagebar.h \
        own_reflection/own_reflect_item.h \
        own_reflection/reflection/ref_base.h \
        own_reflection/reflection/ref_define.h \
        own_reflection/reflection/ref_traits.h \
        own_reflection/tool/ref_helper.h \
        own_reflection/tool/ref_parser.h \
        own_slidestackedview.h \
        own_util.h

FORMS += \
        MainWindow.ui \
        OwnItemUploadView.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    images.qrc \
    qss.qrc
