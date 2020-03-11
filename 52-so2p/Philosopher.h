#ifndef PHILOSOPHER_H
#define PHILOSOPHER_H

#include "Waiter.h"
#include "MyTime.h"

#include <iostream>
#include <thread>
#include <chrono>

#include "PhilosopherInterface.h"

/**
 * @brief The Philosopher class run as thread
 */
class Philosopher : public PhilosopherInterface
{
    Waiter& waiter;
    float value;

    /**
     * @brief state what philosopher doing
     * value 0: thinking
     * value 1: eating
     * value 2: died
     * value 3: waitingForFork
     */
    int state;

    float leftThinkTime;
    float leftEatTime;
    float leftLiveTime;

    void initTimes();
    int id;

public:
    Philosopher(Waiter& w, int id);
    virtual ~Philosopher();
    void init();

    void run();
    int getState() const {return state;}

    float getLeftThinkTime() const {return leftThinkTime;}
    float getLeftEatTime() const {return leftEatTime;}
    float getLeftLiveTime() const {return leftLiveTime;}

    virtual void startEating();
};

#endif // PHILOSOPHER_H
