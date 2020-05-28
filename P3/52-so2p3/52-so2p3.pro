TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt


QMAKE_CXXFLAGS += -lncurses
QMAKE_CXXFLAGS_DEBUG += -lncurses
QMAKE_CXXFLAGS += -std=c++11

LIBS += -L/usr/include -lncurses -lpthread


SOURCES += main.cpp \
    MyTime.cpp

HEADERS += \
    MyTime.h
