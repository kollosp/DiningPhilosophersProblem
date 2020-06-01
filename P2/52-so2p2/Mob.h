#ifndef MOB_H
#define MOB_H

#include "Board.h"
#include "constants.h"
#include "MyTime.h"
#include "Manager.h"
#include "PlayerInterface.h"

class Mob : public PlayerInterface
{


    float xPos = 0;
    float yPos = 0;
    float xSpeed = 4; //10 cells per second
    float ySpeed = 0; //10 cells per second
    float speed = 4; // 10 cells per second
    float timeToHunt;
    float timeToDie;
    int power = 10;
    bool working;

    int id;
    int _lives = 1;

    GameManager& manager;
    MyTimer tm;
    MyTimer liveCircleTimer;


short state;

public:
    Mob(GameManager& mm, int _id);
    virtual ~Mob();

    void changeDirection();

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

    virtual void getDamage(int dmg);

    int getId(){return id;}

    void randomizeTimes();

    virtual int timeLeft();
};

#endif // MOB_H
