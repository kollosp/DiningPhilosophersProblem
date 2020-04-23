#include "MainWindow.h"


MainWindow::MainWindow()
{
    init();
}

MainWindow::~MainWindow()
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

    endwin();
}

void MainWindow::init()
{
    initscr();
    timeout(1);
    noecho();

    start_color();
    init_pair(WALL_PAIR, COLOR_WHITE, COLOR_WHITE);
    init_pair(EMPTY_PAIR, COLOR_BLACK, COLOR_BLACK);
    init_pair(PLAYER_PAIR, COLOR_YELLOW, COLOR_BLACK);
    init_pair(POINT_PAIR, COLOR_YELLOW, COLOR_BLACK);


    board.build(consoleW-2, consoleH-2);

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

    for(unsigned int i=0;i<1;++i){

        PlayerInterface *p = new Player(board);
        threads.push_back(new std::thread(&PlayerInterface::run, p));
        players.push_back(p);
    }

}

void MainWindow::draw()
{
    drawBoard();

    for(PlayerInterface* p : players){
        p->draw(mainwin);
    }

    wmove(mainwin, 0,0);
    wrefresh(mainwin);
}

void MainWindow::drawBoard()
{
    for(unsigned int y=0;y<board.getHeight();++y){

        std::string row="";
        for(unsigned int x=0;x<board.getWidth();++x){
            if(board.get(y,x).checkIfWall()){

                wattron(mainwin,COLOR_PAIR(WALL_PAIR));
                mvwaddstr(mainwin,y+1,x+1,WALL_CHAR);
                wattroff(mainwin, COLOR_PAIR(WALL_PAIR));

            }else{
                if(board.get(y,x).getPoints() > 0){

                    wattron(mainwin, COLOR_PAIR(POINT_PAIR));
                    mvwaddstr(mainwin,y+1,x+1,POINT_CHAR);
                    wattroff(mainwin, COLOR_PAIR(POINT_PAIR));

                }else{
                    wattron(mainwin, COLOR_PAIR(EMPTY_PAIR));
                    mvwaddstr(mainwin,y+1,x+1,EMPTY_CHAR);
                    wattroff(mainwin, COLOR_PAIR(EMPTY_PAIR));
                }
            }
        }
    }

    std::stringstream ss;
    ss<<"Points left: "<<board.getPoints();
    mvwaddstr(mainwin,consoleH-1,1,ss.str().c_str());
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

int MainWindow::run()
{
    working = true;
    while(working){
        run_getch();

        if(board.getPoints() == 0){
            working = 0;
        }

        draw();
        std::this_thread::sleep_for (std::chrono::milliseconds(20));
    }

    return 0;
}

int MainWindow::run_getch()
{
    int c = getChar();

    if(c > 0){
        //std::cout<<c<<(c == 113)<<std::endl;

        //notify players
        for(PlayerInterface* p : players){
            p->notifyKeyPressed(c);
        }

        switch (c) {
        //Q
        case 113:
            working = false;
            break;
        default:
            break;
        }
    }
}

