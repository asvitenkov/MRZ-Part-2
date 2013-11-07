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
        mainwindow.cpp \
    image.cpp \
    #array2d.inl
    neuralnetwork.cpp \
    worker.cpp

HEADERS  += mainwindow.h \
    image.h \
    neuralnetwork.h \
    worker.h

FORMS    += mainwindow.ui

LIBS     += -larmadillo


#QMAKE_LFLAGS_RELEASE += -O2


##### -march=corei7-avx


QMAKE_CXXFLAGS_RELEASE += -march=corei7-avx -DARMA_NO_DEBUG -m64
QMAKE_CFLAGS_RELEASE += -march=corei7-avx -DARMA_NO_DEBUG -m64



#CFLAGS        = -march=corei7-avx -pipe -O2
#CXXFLAGS      = -march=corei7-avx -pipe -O2

#QMAKE_CXXFLAGS_RELEASE += -O1
#QMAKE_CFLAGS_RELEASE += -O1

#QMAKE_CFLAGS_RELEASE += -Os

#QMAKE_CXXFLAGS += -m64
#QMAKE_LFLAGS += -m64
#QMAKE_CFLAGS_RELEASE += m64

