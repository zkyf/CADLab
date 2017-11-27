DEFINES += _USE_MATH_DEFINES

QT += core
QT += gui
QT += opengl
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

TARGET = CADLab
CONFIG -= app_bundle

TEMPLATE = app

DEFINES += QT_DEPRECATED_WARNINGS

SOURCES += main.cpp \
    camera3d.cpp \
    input.cpp \
    light.cpp \
    main.cpp \
    ModelDisplayGL.cpp \
    transform3d.cpp \
    mainwindow.cpp

LIBS += opengl32.lib

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

HEADERS += \
    brep.hpp \
    camera3d.h \
    input.h \
    light.h \
    ModelDisplayGL.h \
    transform3d.h \
    mainwindow.h

RESOURCES += \
    resources.qrc
