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
    neuralnetwork.cpp \
    worker.cpp \
    firstreport.cpp \
    secondreport.cpp \
    defines.cpp \
    thirdreport.cpp \
    fourthreport.cpp

HEADERS  += mainwindow.h \
    image.h \
    neuralnetwork.h \
    worker.h \
    firstreport.h \
    secondreport.h \
    defines.h \
    thirdreport.h \
    fourthreport.h

FORMS    += mainwindow.ui

QMAKE_CXXFLAGS_RELEASE += -march=corei7-avx -DARMA_NO_DEBUG -m64 -DNDEBUG -pthread -O3
QMAKE_CFLAGS_RELEASE += -march=corei7-avx -DARMA_NO_DEBUG -m64 -DNDEBUG -pthread -O3

QMAKE_LFLAGS += -O3

QMAKE_CXXFLAGS_DEBUG += -DARMA_NO_DEBUG
QMAKE_CFLAGS_RELEASE += -DARMA_NO_DEBUG

#unix:!macx:!symbian: LIBS += -L$$PWD/../../../armadillo-3.920.2/ -larmadillo

#INCLUDEPATH += $$PWD/../../../armadillo-3.920.2/include
#DEPENDPATH += $$PWD/../../../armadillo-3.920.2/include



unix:!macx:!symbian: LIBS += -L$$PWD/../../../../../foss/installed/openblas/lib/ -lopenblas

INCLUDEPATH += $$PWD/../../../../../foss/installed/openblas/include
DEPENDPATH += $$PWD/../../../../../foss/installed/openblas/include

unix:!macx:!symbian: LIBS += -L$$PWD/../../../../armadillo-3.920.2/ -larmadillo

INCLUDEPATH += $$PWD/../../../../armadillo-3.920.2/include
DEPENDPATH += $$PWD/../../../../armadillo-3.920.2/include

RESOURCES += \
    images.qrc
