#ifndef PLAYER_H
#define PLAYER_H

#include <iostream>
#include "PlayerInterface.h"
#include "Board.h"
#include "constants.h"
#include "MyTime.h"
#include "Manager.h"

class Player : public PlayerInterface
{
    float xPos = 0;
    float yPos = 0;
    float xSpeed = 0; //10 cells per second
    float ySpeed = 0; //10 cells per second
    float speed = 6; // 10 cells per second
    int power = 10;
    bool working;

    GameManager& manager;

    MyTimer tm;
    int _lives = 3;

public:
    Player(GameManager& mm);
    virtual ~Player(){}

    virtual void draw(WINDOW* wn, int offsetX=0, int offsetY=0);

    virtual void notifyKeyPressed(int key);

    /**
     * Run anybody in parallel thread
     * @brief run
     */

    virtual void end();

    void run();

    virtual void clearClock();

    virtual int getX() {return xPos;}
    virtual int getY() {return yPos;}

    void spawn();

    virtual int lives(){return this->_lives ;}

    virtual void getDamage(int dmg){this->_lives -= dmg;}
};

#endif // PLAYER_H
