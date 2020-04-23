TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

QMAKE_CXXFLAGS += -lncurses
QMAKE_CXXFLAGS_DEBUG += -lncurses
QMAKE_CXXFLAGS += -std=c++11

LIBS += -L/usr/include -lncurses -lpthread



SOURCES += main.cpp \
    Board.cpp \
    MainWindow.cpp \
    PlayerInterface.cpp \
    Player.cpp \
    Mob.cpp \
    Manager.cpp \
    MyTime.cpp

HEADERS += \
    Board.h \
    MainWindow.h \
    PlayerInterface.h \
    Player.h \
    Mob.h \
    Manager.h \
    constants.h \
    MyTime.h
