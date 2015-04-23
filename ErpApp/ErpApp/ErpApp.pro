#-------------------------------------------------
#
# Project created by QtCreator 2015-04-22T14:58:54
#
#-------------------------------------------------

QT       += core gui
QT       += sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ErpApp
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    newskudlg.cpp \
    gfunctions.cpp \
    newproductdlg.cpp \
    mainwidget.cpp \
    SqlManager.cpp

HEADERS  += mainwindow.h \
    newskudlg.h \
    gfunctions.h \
    newproductdlg.h \
    SqlManager.h \
    mainwidget.h

FORMS    += mainwindow.ui \
    newskudlg.ui \
    newproductdlg.ui \
    mainwidget.ui

RESOURCES += \
    resource.qrc
