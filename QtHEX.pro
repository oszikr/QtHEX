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
    hexabcontrol.cpp \
    hexminmaxcontrol.cpp \
    hexnnetcontrol.cpp \
        mainwindow.cpp \
    hexstatespace.cpp \
    hexcanvas.cpp

HEADERS  += mainwindow.h \
    hexabcontrol.h \
    hexminmaxcontrol.h \
    hexnnetcontrol.h \
    hexstatespace.h \
    hexcanvas.h \
    hexagon.hpp

FORMS    += mainwindow.ui

DISTFILES += \
    examplegame.txt
