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
    neuropixel.cpp \
    image.cpp \
    #array2d.inl

HEADERS  += mainwindow.h \
    neuropixel.h \
    image.h \
    array2d.h

FORMS    += mainwindow.ui

LIBS     += -larmadillo
