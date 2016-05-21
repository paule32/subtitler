#-------------------------------------------------
#
# Project created by QtCreator 2016-05-20T21:57:48
#
#-------------------------------------------------

QT += core gui widgets

TARGET = subtiler
TEMPLATE = app

QMAKE_CXXFLAGS += \
        -Wno-unused-parameter \
        -Wno-unused-variable \
        -Wno-unused-local-typedefs \
        -Wno-unused-but-set-variable \
        -Wno-write-strings \
        -Wno-extra \
        -Wunused-function \
        -Woverloaded-virtual -fpermissive \
        -frtti -fexceptions  -std=c++11 \
        -I$${TOPDIR}. -I$${TOPDIR}/source -I$${TOPDIR}/source/includes \
        -D__BYTE_ORDER=__LITTLE_ENDIAN \
        -DQT_DEPRECATED \
        -DQT_DISABLE_DEPRECATED_BEFORE

SOURCES += main.cpp\
        mainwindow.cpp \
    editorgutter.cpp

HEADERS  += mainwindow.h \
    editorgutter.h

FORMS    += mainwindow.ui

DISTFILES += \
    LICENSE
