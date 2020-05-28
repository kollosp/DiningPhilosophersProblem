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
#include "Player.h"
#include "Mob.h"


class MainWindow
{
    int consoleW = 80;
    int consoleH = 24;
    int winposX = 0;
    int winposY = 0;

    Board board;
    WINDOW * mainwin = NULL;

    std::vector<std::thread*> threads;
    std::vector<PlayerInterface*> players;

    bool working;

    int getChar();

    void endAll();

    GameManager *manager;

public:
    MainWindow();
    ~MainWindow();
    void init();
    void initPlayer();

    void spawnMob();

    void draw();
    int run();
    int run_getch();
};

#endif // MAINWINDOW_H
