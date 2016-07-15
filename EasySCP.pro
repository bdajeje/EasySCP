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
    utils/scp.hpp \
    utils/boost/process/detail/file_handle.hpp \
    utils/boost/process/detail/pipe.hpp \
    utils/boost/process/detail/posix_ops.hpp \
    utils/boost/process/detail/stream_info.hpp \
    utils/boost/process/detail/systembuf.hpp \
    utils/boost/process/detail/win32_ops.hpp \
    utils/boost/process/child.hpp \
    utils/boost/process/config.hpp \
    utils/boost/process/context.hpp \
    utils/boost/process/environment.hpp \
    utils/boost/process/operations.hpp \
    utils/boost/process/pistream.hpp \
    utils/boost/process/posix_child.hpp \
    utils/boost/process/posix_context.hpp \
    utils/boost/process/posix_operations.hpp \
    utils/boost/process/posix_status.hpp \
    utils/boost/process/postream.hpp \
    utils/boost/process/process.hpp \
    utils/boost/process/self.hpp \
    utils/boost/process/status.hpp \
    utils/boost/process/stream_behavior.hpp \
    utils/boost/process/win32_child.hpp \
    utils/boost/process/win32_context.hpp \
    utils/boost/process/win32_operations.hpp \
    utils/boost/process.hpp

LIBS += -lboost_filesystem -lboost_system
