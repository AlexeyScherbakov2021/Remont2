QT += testlib sql
QT -= gui

CONFIG += qt console warn_on depend_includepath testcase
CONFIG -= app_bundle

TEMPLATE = app

SOURCES +=  tst_remtest.cpp \
    testbase.cpp

HEADERS += \
    testbase.h

INCLUDEPATH += ../Remont
include(../Remont/Remont.pri)
