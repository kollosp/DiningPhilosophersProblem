#include "Player.h"


Player::Player(Board &b):
    board(b)
{
    working = true;
}

void Player::draw(WINDOW *wn)
{
    wattron(wn,COLOR_PAIR(PLAYER_PAIR));
    mvwprintw(wn,yPos+1,xPos+1,"%s",PLAYER_CHAR);
    wattroff(wn, COLOR_PAIR(PLAYER_PAIR));
}

void Player::notifyKeyPressed(int key)
{
    switch(key){
    //W
    case 119:
        if(board.checkMove(yPos-1, xPos)){
            ySpeed = -speed;
            xSpeed = 0;
        }
        break;
    //S
    case 115:
        if(board.checkMove(yPos+1, xPos)){
            ySpeed = +speed;
            xSpeed = 0;
        }
        break;
    //A
    case 97:
        if(board.checkMove(yPos, xPos-1)){
            ySpeed = 0;
            xSpeed = -speed;
        }
        break;
    //D
    case 100:
        if(board.checkMove(yPos, xPos+1)){
            ySpeed = 0;
            xSpeed = speed;
        }
        break;
    }
}

void Player::run()
{

    MyTimer tm;
    tm.update();

    while(working){
        //mutex
        //...

        int lastx = xPos;
        int lasty = yPos;

        xPos += xSpeed * tm.timeElapsed()/1000.0;
        yPos += ySpeed * tm.timeElapsed()/1000.0;

        if(xPos != lastx){
            if(!board.checkMove(yPos, xPos))
                xPos = lastx;
        }

        if(yPos != lasty){
            if(!board.checkMove(yPos, xPos))
                yPos = lasty;
        }

        if(xPos != lastx || yPos != lasty){
            board.notifyPlayerBeing(yPos,xPos);
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



