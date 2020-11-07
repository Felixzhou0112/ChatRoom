#-------------------------------------------------
#
# Project created by QtCreator 2020-10-24T11:36:27
#
#-------------------------------------------------

QT       += core gui  network  sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ChattingServer
TEMPLATE = app


SOURCES += main.cpp\
        chattingserver.cpp \
    mytcpserver.cpp \
    tcpsocket.cpp \
    protocol.cpp \
    operatedatabase.cpp

HEADERS  += chattingserver.h \
    mytcpserver.h \
    tcpsocket.h \
    protocol.h \
    operatedatabase.h

FORMS    += chattingserver.ui

RESOURCES += \
    resource.qrc
