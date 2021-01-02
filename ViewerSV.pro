QT       += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ViewerSV
TEMPLATE = app

CONFIG += c++17
CONFIG += qwt

# INSTALL ubuntu : sudo apt install libqwt-qt5-dev libqwt-qt5-6
CONFIG(release, release|debug):QMAKE_CXXFLAGS += -Wall -O2
CONFIG(release, release|debug):DESTDIR = release/
CONFIG(release, release|debug):OBJECTS_DIR = release/
CONFIG(release, release|debug):MOC_DIR = release/
CONFIG(release, release|debug):UI_DIR = release/

CONFIG(debug, release|debug):DESTDIR = debug/
CONFIG(debug, release|debug):OBJECTS_DIR = debug/
CONFIG(debug, release|debug):MOC_DIR = debug/
CONFIG(debug, release|debug):UI_DIR = debug/

INCLUDEPATH += /usr/include/eigen3/
LIBS += -lGLEW  -lboost_system -lboost_filesystem -lqwt-qt5

SOURCES += \
    camera.cc \
    cube.cc \
    glwidget.cc \
    main.cc \
    main_window.cc \
    plotsrgba.cpp \
    plotx.cpp \
    volume.cc \
    volume_io.cc

HEADERS  += \
    camera.h \
    cube.h \
    glwidget.h \
    main_window.h \
    plotsrgba.h \
    plotx.h \
    volume.h \
    volume_io.h \

FORMS    += \
    main_window.ui

DISTFILES += \
    shaders/raycast.frag \
    shaders/raycast.vert


