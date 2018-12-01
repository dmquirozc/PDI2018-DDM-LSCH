TEMPLATE = app

LIBS += `pkg-config \
    opencv \
    --cflags \
    --libs`

CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        main.cpp
