#ifndef PLAYER_H
#define PLAYER_H

#include <iostream>
#include "PlayerInterface.h"
#include "Board.h"
#include "constants.h"
#include "MyTime.h"

class Player : public PlayerInterface
{
    float xPos = 0;
    float yPos = 0;
    float xSpeed = 0; //10 cells per second
    float ySpeed = 0; //10 cells per second
    float speed = 6; // 10 cells per second
    int power = 10;
    bool working;

    Board& board;

public:
    Player(Board& b);
    virtual ~Player(){}

    virtual void draw(WINDOW* wn);

    virtual void notifyKeyPressed(int key);

    /**
     * Run anybody in parallel thread
     * @brief run
     */

    virtual void end();

    void run();
};

#endif // PLAYER_H
