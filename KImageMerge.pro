#-------------------------------------------------
#
# Project created by QtCreator 2016-11-10T17:20:07
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
#QMAKE_CXXFLAGS += -O3
CONFIG += c++11

TARGET = KImageMerge
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    kimagecollector.cpp \
    kimagemerge.cpp \
    kcurveanalyse.cpp \
    kfixpointanalyse.cpp \
    krestoration.cpp

HEADERS  += mainwindow.h \
    krawdblintimg.h \
    kimagecollector.h \
    kimagemerge.h \
    kcurveanalyse.h \
    kfixpointanalyse.h \
    kparamcontainer.h \
    krestoration.h

FORMS    += mainwindow.ui
