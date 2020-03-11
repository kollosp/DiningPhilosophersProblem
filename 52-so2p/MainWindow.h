#ifndef MAINWINDOW_H
#define MAINWINDOW_H


#include <ncurses.h>
#include <thread>
#include <vector>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <math.h>
#include "Config.h"
#include "Philosopher.h"

class MainWindow
{
    int consoleW = 80;
    int consoleH = 24;
    int winposX = 0;
    int winposY = 0;

    int tableW = 30;
    int tableH = 22;
    int tableposX = 1;
    int tableposY = 49;

    int logsW = 48;
    int logsH = 8;
    int logsposX = 1;
    int logsposY = 49;

    const Config& config;

    Waiter waiter;

    std::vector<std::thread*> threads;
    std::vector<Philosopher*> philosophers;

    WINDOW * mainwin = NULL;

    WINDOW * tablewin = NULL;
    WINDOW * logswin = NULL;

private:
    void initFrames();
    void initTable();
    void initLogs();
    void drawTable();
    void drawLogs();
    void drawCircle(WINDOW* win, int centerX, int centerY, int r);
    void drawPhilosophersTable();

    std::string stateToString(int state);

public:
    MainWindow(const Config& c);
    ~MainWindow();
    void init();
    void end();

    void draw();
    int run();
};

#endif // MAINWINDOW_H
