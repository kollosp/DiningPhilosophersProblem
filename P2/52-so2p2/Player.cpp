#include "Player.h"


Player::Player(GameManager& mm):
    manager(mm)
{
    spawn();
    ySpeed = speed;
    working = true;
}

void Player::draw(WINDOW *wn, int offsetX, int offsetY)
{
    wattron(wn,COLOR_PAIR(PLAYER_PAIR));
    mvwprintw(wn,yPos + offsetY,xPos + offsetX,"%s",PLAYER_CHAR);
    wattroff(wn, COLOR_PAIR(PLAYER_PAIR));
}

void Player::notifyKeyPressed(int key)
{
    switch(key){
    //W
    case 119:
        if(manager.checkMove(this, yPos-1, xPos)){
            ySpeed = -speed;
            xSpeed = 0;
        }
        break;
    //S
    case 115:
        if(manager.checkMove(this, yPos+1, xPos)){
            ySpeed = +speed;
            xSpeed = 0;
        }
        break;
    //A
    case 97:
        if(manager.checkMove(this, yPos, xPos-1)){
            ySpeed = 0;
            xSpeed = -speed;
        }
        break;
    //D
    case 100:
        if(manager.checkMove(this, yPos, xPos+1)){
            ySpeed = 0;
            xSpeed = speed;
        }
        break;
    }
}

void Player::run()
{

    tm.update();

    while(working){
        //mutex
        //...

        int lastx = xPos;
        int lasty = yPos;

        xPos += xSpeed * tm.timeElapsed()/1000.0;
        yPos += ySpeed * tm.timeElapsed()/1000.0;

        if(xPos != lastx){
            if(!manager.checkMove(this, yPos, xPos))
                xPos = lastx;
        }

        if(yPos != lasty){
            if(!manager.checkMove(this, yPos, xPos))
                yPos = lasty;
        }

        if(xPos != lastx || yPos != lasty){
            manager.notifyPlayerBeing(this, yPos,xPos);
        }

        //mutex unlock
        tm.update();
        std::this_thread::sleep_for (std::chrono::milliseconds(20));
        //mutex lock
    }
}

void Player::end(){

    //mutex lock in function
    working = false;
}

void Player::clearClock()
{
    tm.update();
}

void Player::spawn()
{
    int x, y;
    do{
        y = rand()%(manager.getBoard().getHeight()-1) + 1;
        x = rand()%(manager.getBoard().getWidth()-1) + 1;
    }while(manager.getBoard().checkMove(y,x) == false);

    yPos = y;
    xPos = x;
    xSpeed = 0;
    ySpeed = 0;
}


