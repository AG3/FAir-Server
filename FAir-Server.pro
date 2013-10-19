#-------------------------------------------------
#
# Project created by QtCreator 2013-10-04T23:40:58
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = FAir-Server
TEMPLATE = app


SOURCES += main.cpp\
        serverbase.cpp \
    FTcpConnection.cpp

HEADERS  += serverbase.h \
    FTcpConnection.h

FORMS    += serverbase.ui
