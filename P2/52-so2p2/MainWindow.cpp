#include "MainWindow.h"


MainWindow::MainWindow()
{
    init();
}

MainWindow::~MainWindow()
{

    endwin();
    delete manager;
}

void MainWindow::init()
{
    initscr();
    timeout(1);
    noecho();

    start_color();
    init_pair(WALL_PAIR, COLOR_WHITE, COLOR_WHITE);
    init_pair(EMPTY_PAIR, COLOR_BLACK, COLOR_BLACK);
    init_pair(PLAYER_PAIR, COLOR_BLACK, COLOR_YELLOW);
    init_pair(POINT_PAIR, COLOR_YELLOW, COLOR_BLACK);
    init_pair(MOB_PAIR, COLOR_BLACK, COLOR_RED);
    init_pair(MOB_HUNT_PAIR, COLOR_BLACK, COLOR_MAGENTA);
    init_pair(MOB_WAITING_PAIR, COLOR_BLACK, COLOR_BLUE);
    init_pair(SUPER_POINT_PAIR, COLOR_BLUE, COLOR_BLACK);
    init_pair(SUPER_POINT_2_PAIR, COLOR_RED, COLOR_BLACK);



    board.build(consoleW/2, consoleH/2);

    std::string title = "Pacman";
    std::string footer = "build: " + std::string(__DATE__) + " " + std::string(__TIME__);

    mainwin = newwin(consoleH,consoleW,winposX,winposY);
    refresh();

    box(mainwin, 0, 0);

    mvwprintw(mainwin, 0, (consoleW-title.size())/2, "%s", title.c_str());
    mvwprintw(mainwin, consoleH-1, (consoleW-footer.size())-1, "%s", footer.c_str());

    wrefresh(mainwin);

    initPlayer();
}

void MainWindow::initPlayer()
{
    manager = new GameManager(board);

    for(unsigned int i=0;i<1;++i){
        PlayerInterface *p = new Player(*manager);
        threads.push_back(new std::thread(&PlayerInterface::run, p));
        players.push_back(p);
    }

    manager->setPlayer(players[0]);

    for(unsigned int i=0;i<5;++i){
        spawnMob();
    }
}

void MainWindow::spawnMob()
{
    PlayerInterface *p = new Mob(*manager, players.size());
    threads.push_back(new std::thread(&PlayerInterface::run, p));
    players.push_back(p);
    manager->pushBackMob(p);
}

void MainWindow::draw()
{
    board.draw(mainwin, 1, 1);

    std::stringstream ss;
    ss<<"Points left: "<<setw(4)<<board.getPoints()<<" | lives: "<<
        manager->getPlayer()->lives()<<" | "<<manager->getHunters();
    mvwaddstr(mainwin,consoleH-1,1,ss.str().c_str());

    for(PlayerInterface* p : players){
        p->draw(mainwin, 1,1);
    }

    wmove(mainwin, 0,0);
    wrefresh(mainwin);
}

int MainWindow::getChar()
{
    int ch = getch();

    if (ch != ERR) {
        return ch;
    } else {
        return 0;
    }
}

void MainWindow::endAll()
{
    for(PlayerInterface* p : players){
        p->end();
    }

    for(std::thread* t : threads){
            t->join();
    }

    for(PlayerInterface* p : players){
        delete p;
    }
}

int MainWindow::run()
{
    working = true;
    while(working){
        run_getch();

        if(board.getPoints() < 5 ||
           manager->getPlayer()->lives() == 0){
            working = 0;
            manager->unlockMobs();
        }

        switch(manager->manage()){
        case 1:
            if(players.size()<MAX_MOB_COUNT + 1 && MODE == 2)
                spawnMob();
            break;
        }

        draw();
        std::this_thread::sleep_for (std::chrono::milliseconds(20));
    }

    manager->unlockMobs();
    endAll();
    return 0;
}

int MainWindow::run_getch()
{
    int c = getChar();

    if(c > 0){

        //notify players
        for(PlayerInterface* p : players){
            p->notifyKeyPressed(c);
        }

        switch (c) {
        //Q
        case 113:
            working = false;
            manager->unlockMobs();
            break;

        case 'e':
            endAll();
            break;

        //lock mobs
        case 'z':
            manager->lockMobs();
            break;
        default:
            break;
        }
    }
}

