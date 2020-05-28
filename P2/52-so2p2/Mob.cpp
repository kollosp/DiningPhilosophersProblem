#include "Mob.h"

Mob::Mob(GameManager &mm, int _id):
    manager(mm), id(_id)
{
    int x, y;
    do{
        y = rand()%(manager.getBoard().getHeight()-1) + 1;
        x = rand()%(manager.getBoard().getWidth()-1) + 1;
    }while(manager.checkMove(this, y,x) == false);

    yPos = y;
    xPos = x;

    state = 0;
    working = true;
    randomizeTimes();
}

Mob::~Mob(){}

void Mob::changeDirection()
{
    bool directionChanged = false;
    int dir;

    do{
        //up, right, down, left
        dir = rand()%4;
        int bufCoord;

        switch (dir) {
        case 0:
            bufCoord = yPos -1;
            if(manager.checkMove(this, bufCoord, xPos)){
                directionChanged = true;
                ySpeed = -speed;
                xSpeed = 0;
            }
            break;
        case 1:
            bufCoord = xPos + 1;
            if(manager.checkMove(this,yPos, bufCoord)){
                directionChanged = true;
                ySpeed = 0;
                xSpeed = +speed;
            }
            break;
        case 2:
            bufCoord = yPos + 1;
            if(manager.checkMove(this, bufCoord, xPos)){
                directionChanged = true;
                ySpeed = speed;
                xSpeed = 0;
            }
            break;
        case 3:
            bufCoord = xPos - 1;
            if(manager.checkMove(this, yPos, bufCoord)){
                directionChanged = true;
                ySpeed = 0;
                xSpeed = -speed;
            }
            break;
        default:
            break;
        }

    }while(directionChanged == false);
}

void Mob::draw(WINDOW *wn, int offsetX, int offsetY)
{
    if(_lives){

        switch(state){
        case 0:

            wattron(wn,COLOR_PAIR(MOB_PAIR));
            mvwprintw(wn,yPos+offsetY,xPos+offsetX,"%c",(id+'0'-1));
            wattroff(wn, COLOR_PAIR(MOB_PAIR));

            break;
        case 1:

            wattron(wn,COLOR_PAIR(MOB_WAITING_PAIR));
            mvwprintw(wn,yPos+offsetY,xPos+offsetX,"%c",(id+'0'-1));
            wattroff(wn, COLOR_PAIR(MOB_WAITING_PAIR));

            break;
        case 2:


            wattron(wn,COLOR_PAIR(MOB_HUNT_PAIR));
            mvwprintw(wn,yPos+offsetY,xPos+offsetX,"%c",(id+'0'-1));
            wattroff(wn, COLOR_PAIR(MOB_HUNT_PAIR));
            break;
        }

    }
}

void Mob::notifyKeyPressed(int key)
{
    //manager allow to eat
    if(key == 0){
        state = 2;
    }
}

void Mob::end()
{
    working = false;
}

void Mob::run()
{

    tm.update();
    liveCircleTimer.update();

    while(working){

        std::unique_lock<std::mutex>* lck = manager.mobWait();

        int lastx = xPos;
        int lasty = yPos;

        xPos += xSpeed * tm.timeElapsed()/1000.0;
        yPos += ySpeed * tm.timeElapsed()/1000.0;

        if(xPos != lastx){
            if(!manager.checkMove(this, yPos, xPos)){
                xPos = lastx;
                changeDirection();
            }
        }

        if(yPos != lasty){
            if(!manager.checkMove(this, yPos, xPos)){
                yPos = lasty;
                changeDirection();
            }
        }

        float timeElapsed = liveCircleTimer.timeElapsed();

        if(timeElapsed > timeToDie){
            state = 3;
        }

        switch (state) {
        //simple walking
        case 0:
            if(timeElapsed > timeToHunt){
                state = 1;
                manager.askForHunt(this);
            }

           break;
        //waiting for hunt permission
        case 1:
            //do nothing wait for manager signal
            break;
        //hunting
        case 2:

            if(xPos != lastx || yPos != lasty){
                if(manager.notifyPlayerBeing(this, yPos,xPos) == 1){
                    liveCircleTimer.update();
                    state = 0;
                    randomizeTimes();
                }
            }

            break;
        //died
        case 3:
            _lives = 0;
            working = false;
            xPos = -1;
            yPos = -1;

            manager.notifyPlayerDied(this);
            break;

        default:
            break;
        }

        //mutex unlock
        tm.update();
        manager.mobNotify(lck);
        std::this_thread::sleep_for (std::chrono::milliseconds(20));
        //mutex lock
    }
}

void Mob::clearClock()
{
    tm.update();
}

void Mob::getDamage(int dmg)
{
    _lives = 0;
    working = false;
    xPos = -1;
    yPos = -1;
    manager.notifyPlayerDied(this);
}

void Mob::randomizeTimes()
{
    timeToHunt = rand() % 15000;
    timeToDie = rand() % 25000 + timeToHunt;
}
