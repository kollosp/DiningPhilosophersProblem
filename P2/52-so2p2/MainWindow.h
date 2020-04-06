#ifndef MAINWINDOW_H
#define MAINWINDOW_H


#include <ncurses.h>
#include <thread>
#include <vector>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <math.h>

#include "Board.h"

class MainWindow
{
    Board board();

public:
    MainWindow(const Config& c);
    ~MainWindow();
    void init();

    void draw();
    int run();
};

#endif // MAINWINDOW_H
