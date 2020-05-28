#ifndef PLAYERINTERFACE_H
#define PLAYERINTERFACE_H


#include <ncurses.h>
#include <thread>

class PlayerInterface
{
public:
    PlayerInterface();
    virtual ~PlayerInterface(){}

    /**
     * Used by manager to determinate who win the fight
     * @brief getPower
     * @return
     */
    int getPower();

    virtual int getX() = 0;
    virtual int getY() = 0;

    /**
     * Make next move
     * @brief nextMove
     * @return
     */
    int nextMove();


    /**
     * Function draws player or mob in its specified way
     * @brief draw
     * @param wn
     */
    virtual void draw(WINDOW* wn, int offsetX=0, int offsetY=0) = 0;

    /**
     * Notify that somenthink happend
     * @brief notify
     */
    virtual void notifyKeyPressed(int key) = 0;


    /**
     * Run anybody in parallel thread
     * @brief run
     */
    virtual void run() = 0;

    virtual void end() = 0;

    virtual void clearClock() = 0;

    virtual void spawn(){}
    virtual void getDamage(int dmg){}
    virtual int lives(){return 0;}

    virtual int getId(){return 0;}
};

#endif // PLAYERINTERFACE_H
