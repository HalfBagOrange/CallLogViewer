#-------------------------------------------------
#
# Project created by QtCreator 2015-08-15T20:13:42
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = CallLogViewer
TEMPLATE = app


SOURCES += main.cpp \
    barchart.cpp \
    CallLogViewer.cpp \
    ChartDialog.cpp

HEADERS  += \
    barchart.h \
    CallLogViewer.h \
    ChartDialog.h

FORMS += \
    CallLogViewer.ui \
    ChartDialog.ui
