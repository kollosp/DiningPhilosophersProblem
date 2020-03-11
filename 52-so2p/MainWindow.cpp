#include "MainWindow.h"

void MainWindow::initFrames()
{


    std::string title = "Philosophers problem";
    std::string footer = "build: " + std::string(__DATE__) + " " + std::string(__TIME__);

    mainwin = newwin(consoleH,consoleW,winposX,winposY);
    refresh();

    box(mainwin, 0, 0);

    mvwprintw(mainwin, 0, (consoleW-title.size())/2, "%s", title.c_str());
    mvwprintw(mainwin, consoleH-1, (consoleW-footer.size())-1, "%s", footer.c_str());

    wrefresh(mainwin);
}

void MainWindow::initTable()
{
    tableposX = 1;
    tableposY = consoleW - tableW -1;

    tablewin = newwin(tableH,tableW,tableposX,tableposY);
    box(tablewin, 0, 0);

    std::string title = "Philosophers' state";
    mvwprintw(tablewin, 0,(tableW - title.size()) /2, "%s", title.c_str());
    mvwprintw(tablewin, 1, 1, "  Nb   Eat  Thin   Die  Job");
    mvwprintw(tablewin, 2, 1, "   1  24.5  12.7  78.4    E");
    wrefresh(tablewin);

}

void MainWindow::initLogs()
{
    logsposX = consoleH - logsH - 1;
    logsposY = 1;

    logswin = newwin(logsH,logsW,logsposX,logsposY);
    box(logswin, 0, 0);

    std::string title = "Waiter's logs";
    mvwprintw(logswin, 0,(logsW - title.size()) /2, "%s", title.c_str());
    wrefresh(logswin);
}

void MainWindow::drawTable()
{
    for(unsigned int i=0;i<philosophers.size();++i){
        std::stringstream ss;
        ss << std::setw(4)<<(i+1)<<"  "<<
              std::setw(4)<<std::fixed<<std::setprecision(1)<<philosophers[i]->getLeftEatTime()<< "  " <<
              std::setw(4)<<std::fixed<<std::setprecision(1)<<philosophers[i]->getLeftThinkTime()<< "  " <<
              std::setw(4)<<std::fixed<<std::setprecision(1)<<philosophers[i]->getLeftLiveTime()<< "  ";

        ss<<"  "<<stateToString(philosophers[i]->getState());

        mvwprintw(tablewin, i+2, 1, ss.str().c_str());
    }

    wrefresh(tablewin);
}

void MainWindow::drawLogs()
{
    const std::vector<std::string>& logs = waiter.getLogs();

    int size = logsW-2;

    for(unsigned int i=0;i<logs.size();++i){
        std::string s;
        s.append(logs[i]);

        s.append(size-logs[i].size(), '.');

        mvwprintw(logswin, i+1, 1, s.c_str());
    }
    wrefresh(logswin);

}

void MainWindow::drawPhilosophersTable()
{
    int centerX = (consoleW - tableW)/2;
    int centerY = (consoleH - logsH)/2;
    int r = centerY-3;

    drawCircle(mainwin, centerX, centerY, r);

    float alpha = - M_PI/2.0;
    r = centerY-1;
    centerX = centerX-1; //because printed string i 3 char long
    for(int i=0;i<philosophers.size();++i){
        stringstream ss;
        ss<<stateToString(philosophers[i]->getState())<<i+1<<".";
        mvwprintw(mainwin, sin(alpha)*r + centerY, 2.2*cos(alpha)*r + centerX, ss.str().c_str());
        alpha += 2.0*M_PI / philosophers.size();
    }
    wrefresh(mainwin);
}

std::string MainWindow::stateToString(int state)
{

    switch(state){
    case 0: return "T"; break;
    case 1: return "E"; break;
    case 2: return "D"; break;
    case 3: return "W"; break;
    default: return "U"; break;
    }
}

void MainWindow::drawCircle(WINDOW* win, int centerX, int centerY, int r)
{
    float alpha = - M_PI/2.0;

    int size = 52;
    for(int i=0;i<size;++i){
        mvwprintw(win, sin(alpha)*r + centerY, 2.2*cos(alpha)*r + centerX, "#");
        alpha += 2.0*M_PI / size;
    }
    wrefresh(win);
}

MainWindow::MainWindow(const Config& c) : config(c)
{
    waiter.setConfig(c.liveTime, c.eatTime, c.thinkTime);
    init();
}

MainWindow::~MainWindow()
{
    waiter.setWorkingStatus(false);

    for(std::thread* t : threads){
        t->join();
    }

    for(Philosopher* p:philosophers){
        delete p;
    }

    endwin();
}

void MainWindow::init()
{
    initscr();			/* Start curses mode 		  */
    noecho();
    std::vector<PhilosopherInterface*> pi;
    for(unsigned int i=0;i<config.philosofersSize;++i){

        Philosopher *p = new Philosopher(waiter, i);
        threads.push_back(new std::thread(&Philosopher::run, p));
        philosophers.push_back(p);
        pi.push_back(p);
    }

    waiter.setPhilosophers(pi);

    initFrames();
    initTable();
    initLogs();
}

void MainWindow::draw()
{
    drawTable();
    drawLogs();
    drawPhilosophersTable();
}


int MainWindow::run()
{

    while(true){
        draw();
        std::this_thread::sleep_for (std::chrono::milliseconds(20));
    }




    return 0;
}
