#-------------------------------------------------
#
# Project created by QtCreator 2015-10-04T22:44:20
#
#-------------------------------------------------

QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = cangku
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    welcomebuy.cpp \
    welcomesell.cpp \
    welcomecheck.cpp

HEADERS  += mainwindow.h \
    welcomebuy.h \
    welcomesell.h \
    welcomecheck.h

FORMS    += mainwindow.ui \
    welcomebuy.ui \
    welcomesell.ui \
    welcomecheck.ui
