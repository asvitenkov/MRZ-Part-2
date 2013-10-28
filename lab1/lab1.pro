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
