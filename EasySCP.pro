#-------------------------------------------------
#
# Project created by QtCreator 2016-07-14T10:51:28
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = EasySCP
TEMPLATE = app
CONFIG += c++11

SOURCES += main.cpp\
        main_window.cpp \
    windows/choose_file.cpp \
    windows/choose_target.cpp \
    windows/transfert_progress.cpp \
    utils/file.cpp \
    utils/settings.cpp \
    worker/scp_worker.cpp \
    utils/scp.cpp

HEADERS  += main_window.hpp \
    windows/choose_file.hpp \
    windows/choose_target.hpp \
    windows/transfert_progress.hpp \
    utils/file.hpp \
    utils/settings.hpp \
    utils/fonts.hpp \
    worker/scp_worker.hpp \
    utils/scp.hpp

LIBS += -lboost_filesystem -lboost_system
