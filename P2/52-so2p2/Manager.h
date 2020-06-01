#ifndef MANAGER_H
#define MANAGER_H

#include <mutex>
#include <condition_variable>
#include <vector>
#include "PlayerInterface.h"
#include "MyTime.h"
#include "Board.h"

class GameManager
{

    std::vector<PlayerInterface*> hunters;

    std::mutex checkPositionMutex;
    std::mutex mobMutext;
    std::condition_variable mobCV;

    bool mobsLocked = false;
    std::unique_lock<std::mutex>* lockMobsLck;
    std::vector<PlayerInterface*> mobs;
    std::vector<PlayerInterface*> allCreatures;
    PlayerInterface* player;
    MyTimer tm;
    Board &board;

    bool superPlayer = false;
    MyTimer superPlayerTimer;

    /**
     * 0 - nothing
     * 1 - spaw new mob
     * @brief toDoInMain
     */

    int toDoInMain = 0;

public:
    GameManager(Board &b);
    ~GameManager();

    /**
     * @brief conditional variable
     */
    std::unique_lock<std::mutex>* mobWait();
    void mobNotify(std::unique_lock<std::mutex> *lk);

    std::mutex& getMobMutex() {return mobMutext;}
    std::condition_variable& getMobCV() {return mobCV;}

    void lockMobs();

    void lock();
    void unlock();

    /**
     * @brief manage function should be run once per main thread loop
     */
    int manage();
    void pushBackMob(PlayerInterface* p);
    void setPlayer(PlayerInterface* p);

    int notifyPlayerBeing(PlayerInterface *p, unsigned int y, unsigned int x);
    bool checkMove(PlayerInterface *p, unsigned int y, unsigned int x);

    Board& getBoard(){return board;}

    PlayerInterface* getPlayer(){return player;}
    void unlockMobs();

    void askForHunt(PlayerInterface *p);

    string getHunters();

    void notifyPlayerDied(PlayerInterface* p);
};

#endif // MANAGER_H
