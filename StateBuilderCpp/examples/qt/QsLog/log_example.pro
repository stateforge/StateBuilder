# -------------------------------------------------
# Project created by QtCreator 2010-03-20T12:17:43
# -------------------------------------------------
QT -= gui
TARGET = log_example
CONFIG += console
CONFIG -= app_bundle
TEMPLATE = app
DEFINES += QS_LOG_LINE_NUMBERS
SOURCES += main.cpp \
    QsLogDest.cpp \
    QsLog.cpp \
    QsDebugOutput.cpp
HEADERS += QSLogDest.h \
    QsLog.h \
    QsDebugOutput.h
