#-------------------------------------------------
#
# Project created by QtCreator 2016-10-15T15:43:33
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
#QMAKE_CXXFLAGS += -O3
CONFIG += c++11
TARGET = NUC
TEMPLATE = app

SOURCES += main.cpp\
        mainwindow.cpp \
    klabel.cpp \
    mainform.cpp

HEADERS  += mainwindow.h \
    klabel.h \
    KRawDblIntImg.h \
    qlineradjust.h \
    fftpf.h \
    fftpf-impl.h \
    KAdjust.h \
    KGammaAdjust.h \
    KGlapowAdjust.h \
    KLinearAdjust.h \
    KMaskDodAdjust.h \
    KNakaAdjust.h \
    KWallisAdjust.h \
    qadjust.h \
    qnakaadjust.h \
    qgammaadjust.h \
    qglapowadjust.h \
    qhomomorphicfilter.h \
    qmaskdodadjust.h \
    qwallisadjust.h \
    mainform.h \
    khomomorphicfilter.h \
    khomofilter2.h \
    qhomofilter2.h \
    khomofilter3.h \
    qhomofilter3.h \
    khomofilter4.h \
    qhomofilter4.h \
    qautogamma.h \
    kautogamma.h \
    qestimatebkg.h \
    kestimatebkg.h \
    klocalgammaadjust.h \
    qlocalgammaadjust.h \
    kgumadjust.h \
    qgumadjust.h \
    qmultiadjust.h \
    kbkgfit.h \
    qbkgfit.h \
    qgammafit.h \
    kgammafit.h

FORMS    += mainwindow.ui \
    mainform.ui
