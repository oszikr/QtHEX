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
    hexnnetcontrol.cpp \
        mainwindow.cpp \
    hexstatespace.cpp \
    test.cpp \
    hexcanvas.cpp \
    hexstrategycontrol.cpp

HEADERS  += mainwindow.h \
    hexnnetcontrol.h \
    hexstatespace.h \
    hexcanvas.h \
    hexagon.hpp \
    hexstrategycontrol.h

FORMS    += mainwindow.ui
