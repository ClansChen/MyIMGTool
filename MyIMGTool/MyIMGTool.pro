#-------------------------------------------------
#
# Project created by QtCreator 2016-06-18T21:48:32
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = MyIMGTool
TEMPLATE = app


SOURCES += main.cpp\
        MyIMGTool.cpp \
    CKeyGen.cpp \
    IMGClass.cpp \
    IMGTableModel.cpp \
    MyProgressDialog.cpp

HEADERS  += MyIMGTool.h \
    CKeyGen.h \
    IMGClass.h \
    IMGTableModel.h \
    MyProgressDialog.h

FORMS    += MyIMGTool.ui

RESOURCES += \
    MyIMGTool.qrc
