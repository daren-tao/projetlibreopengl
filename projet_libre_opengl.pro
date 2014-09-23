#-------------------------------------------------
#
# Project created by QtCreator 2014-08-03T10:43:51
#
#-------------------------------------------------

QT       += core gui
QT       += opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = projet_libre_opengl
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    mainpanel.cpp

HEADERS  += mainwindow.h \
    mainpanel.h \
    observable.h \
    observer.h

FORMS    += mainwindow.ui

RESOURCES += \
    shaders.qrc

CONFIG += c++11
