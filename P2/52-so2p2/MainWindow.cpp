#include "MainWindow.h"


MainWindow::MainWindow(const Config& c) : config(c)
{
    init();
}

MainWindow::~MainWindow()
{

    endwin();
}

void MainWindow::init()
{
    initscr();			/* Start curses mode 		  */
    noecho();

    initBoard();

}

void MainWindow::draw()
{

}


int MainWindow::run()
{

    while(true){
        draw();
        std::this_thread::sleep_for (std::chrono::milliseconds(20));
    }

    return 0;
}

