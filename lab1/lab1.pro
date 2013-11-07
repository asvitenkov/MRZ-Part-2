#-------------------------------------------------
#
# Project created by QtCreator 2013-10-26T13:12:07
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = lab1
TEMPLATE = app


SOURCES += main.cpp\
    image.cpp \
    neuralnetwork.cpp

HEADERS  +=    image.h \
    neuralnetwork.h

#FORMS    += mainwindow.ui

#LIBS     += -larmadillo


#QMAKE_LFLAGS_RELEASE += -O2


##### -march=corei7-avx


QMAKE_CXXFLAGS_RELEASE += -march=corei7-avx -DARMA_NO_DEBUG -m64 -DNDEBUG -pthread -O3
QMAKE_CFLAGS_RELEASE += -march=corei7-avx -DARMA_NO_DEBUG -m64 -DNDEBUG -pthread -O3

QMAKE_LFLAGS += -O3


#CFLAGS        = -march=corei7-avx -pipe -O2
#CXXFLAGS      = -march=corei7-avx -pipe -O2

#QMAKE_CXXFLAGS_RELEASE += -O1
#QMAKE_CFLAGS_RELEASE += -O1

#QMAKE_CFLAGS_RELEASE += -Os

#QMAKE_CXXFLAGS += -m64
#QMAKE_LFLAGS += -m64
#QMAKE_CFLAGS_RELEASE += m64


unix:!macx:!symbian: LIBS += -L$$PWD/../../../armadillo-3.920.2/ -larmadillo

INCLUDEPATH += $$PWD/../../../armadillo-3.920.2/include
DEPENDPATH += $$PWD/../../../armadillo-3.920.2/include

unix:!macx:!symbian: LIBS += -L$$PWD/../../../../foss/installed/openblas/lib/ -lopenblas

INCLUDEPATH += $$PWD/../../../../foss/installed/openblas/include
DEPENDPATH += $$PWD/../../../../foss/installed/openblas/include

