#-------------------------------------------------
#
# Project created by QtCreator 2024-06-28T11:56:59
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
CONFIG += c++11
TARGET = FiveNodes
TEMPLATE = app
LIBS+= -lWs2_32
# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0
INCLUDEPATH+=$$PWD/Sec_search
include($$PWD/Sec_search/Sec_search.pri)

SOURCES += \
        main.cpp \
        dialog.cpp \
    Inet/TCPClient.cpp \
    Inet/TCPServer.cpp \
    Inet/UDP.cpp \
    Mediator/TCPMediator.cpp \
    Kernel.cpp \
    md5/md5.cpp \
    playhall.cpp \
    gamebotton.cpp \
    gamezone.cpp \
    friendlist.cpp \
    room.cpp \
    gameroom.cpp \
    fivenodeswidget.cpp

HEADERS += \
        dialog.h \
    Inet/Inet.h \
    Inet/Inet_config.h \
    Inet/TCPClient.h \
    Inet/TCPServer.h \
    Inet/UDP.h \
    Mediator/NetMediator.h \
    Mediator/TCPMediator.h \
    Kernel.h \
    protocl.h \
    md5/md5.h \
    playhall.h \
    gamebotton.h \
    gamezone.h \
    hallconfig.h \
    friendlist.h \
    room.h \
    gameroom.h \
    fivenodeswidget.h

FORMS += \
        dialog.ui \
    playhall.ui \
    gamebotton.ui \
    gamezone.ui \
    friendlist.ui \
    room.ui \
    gameroom.ui \
    fivenodeswidget.ui

DISTFILES +=

RESOURCES += \
    res/res.qrc
