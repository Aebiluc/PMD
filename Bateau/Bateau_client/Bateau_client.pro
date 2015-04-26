#-------------------------------------------------
#
# Project created by QtCreator 2015-03-14T11:33:32
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Bateau_client
TEMPLATE = app

SOURCES += main.cpp\
        mainwindow.cpp \
        connection.cpp

HEADERS  += mainwindow.h \
        connection.h

FORMS    += mainwindow.ui
