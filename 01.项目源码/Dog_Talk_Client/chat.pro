#-------------------------------------------------
#
# Project created by QtCreator 2021-08-26T15:27:13
#
#-------------------------------------------------

QT       += core gui network sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = chat
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

SOURCES += \
        main.cpp \
        login.cpp \
    mainwindow.cpp \
    enroll.cpp \
    tcpclientsocket.cpp \
    talkwindow.cpp \
    background.cpp \
    udpreciver.cpp \
    udpsender.cpp

HEADERS += \
        login.h \
    mainwindow.h \
    enroll.h \
    tcpclientsocket.h \
    talkwindow.h \
    background.h \
    udpreciver.h \
    udpsender.h

FORMS += \
        login.ui \
    mainwindow.ui \
    enroll.ui \
    talkwindow.ui \
    background.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    image.qrc

DISTFILES += \
    image/main41.jpg
