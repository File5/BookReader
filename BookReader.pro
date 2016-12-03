#-------------------------------------------------
#
# Project created by QtCreator 2016-10-03T19:37:46
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = BookReader
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    pagedtextedit.cpp \
    book.cpp \
    bookbuilder.cpp \
    bookinfodialog.cpp \
    loadchaptersdialog.cpp \
    addreferencedialog.cpp \
    settingsmanager.cpp \
    settingsdialog.cpp

HEADERS  += mainwindow.h \
    pagedtextedit.h \
    book.h \
    bookbuilder.h \
    bookinfodialog.h \
    loadchaptersdialog.h \
    addreferencedialog.h \
    settingsmanager.h \
    settingsdialog.h

FORMS    += mainwindow.ui \
    bookinfodialog.ui \
    loadchaptersdialog.ui \
    addreferencedialog.ui \
    settingsdialog.ui

RESOURCES += \
    testdata.qrc
