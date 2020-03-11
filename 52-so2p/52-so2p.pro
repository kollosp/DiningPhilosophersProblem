TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

QMAKE_CXXFLAGS += -lncurses
QMAKE_CXXFLAGS_DEBUG += -lncurses
QMAKE_CXXFLAGS += -std=c++11

LIBS += -L/usr/include -lncurses -lpthread


SOURCES += main.cpp \
    MainWindow.cpp \
    Philosopher.cpp \
    Config.cpp \
    Waiter.cpp \
    MyTime.cpp \
    PhilosopherInterface.cpp

HEADERS += \
    MainWindow.h \
    Philosopher.h \
    Config.h \
    Waiter.h \
    MyTime.h \
    PhilosopherInterface.h
