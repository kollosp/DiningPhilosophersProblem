#ifndef PLAYERINTERFACE_H
#define PLAYERINTERFACE_H


#include <ncurses.h>
#include <thread>

class PlayerInterface
{
public:
    PlayerInterface();
    virtual ~PlayerInterface(){}

    int getX();
    int getY();

    /**
     * Used by manager to determinate who win the fight
     * @brief getPower
     * @return
     */
    int getPower();


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
    virtual void draw(WINDOW* wn) = 0;

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
};

#endif // PLAYERINTERFACE_H
