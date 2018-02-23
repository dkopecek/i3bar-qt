#-------------------------------------------------
#
# Project created by QtCreator 2018-01-14T12:59:35
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = i3bar-qt
TEMPLATE = app

CONFIG += console

SOURCES += main.cpp\
        Bar.cpp \
    BarManager.cpp \
    BarWidget.cpp \
    CalendarWidget.cpp

HEADERS  += Bar.hpp \
    i3ipc-qt/i3ipc.hpp \
    BarManager.hpp \
    BarWidget.hpp \
    CalendarWidget.hpp

FORMS    += \
    Bar.ui \
    CalendarWidget.ui

DISTFILES += \
    i3bar-qt.qss
