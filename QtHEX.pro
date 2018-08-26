#-------------------------------------------------
#
# Project created by QtCreator 2018-07-31T12:57:54
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = QtHEX
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    hexstatespace.cpp \
    test.cpp \
    hexcanvas.cpp

HEADERS  += mainwindow.h \
    hexstatespace.h \
    hexcanvas.h \
    hexagon.hpp

FORMS    += mainwindow.ui
