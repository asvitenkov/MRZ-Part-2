#-------------------------------------------------
#
# Project created by QtCreator 2013-12-28T17:16:07
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = lab2
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    defines.cpp \
    neuralnetwork.cpp

HEADERS  += mainwindow.h \
    defines.h \
    neuralnetwork.h

FORMS    += mainwindow.ui


QMAKE_CXXFLAGS_RELEASE += -march=corei7-avx -DARMA_NO_DEBUG -m64 -DNDEBUG -pthread -O3
QMAKE_CFLAGS_RELEASE += -march=corei7-avx -DARMA_NO_DEBUG -m64 -DNDEBUG -pthread -O3

QMAKE_LFLAGS += -O3

QMAKE_CXXFLAGS_DEBUG += -DARMA_NO_DEBUG
QMAKE_CFLAGS_RELEASE += -DARMA_NO_DEBUG


unix:!macx:!symbian: LIBS += -L$$PWD/../../../../../foss/installed/openblas/lib/ -lopenblas

INCLUDEPATH += $$PWD/../../../../../foss/installed/openblas/include
DEPENDPATH += $$PWD/../../../../../foss/installed/openblas/include

unix:!macx:!symbian: LIBS += -L$$PWD/../../../../armadillo-3.920.2/ -larmadillo

INCLUDEPATH += $$PWD/../../../../armadillo-3.920.2/include
DEPENDPATH += $$PWD/../../../../armadillo-3.920.2/include
