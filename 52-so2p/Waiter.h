#ifndef WAITER_H
#define WAITER_H

#include <vector>
#include <mutex>

#include "PhilosopherInterface.h"
#include <sstream>

class Waiter
{
    bool working;
    std::mutex logsMutex;

public:
    class Config
    {
    public:
        Config(float liveTime=60, float eatTime=20, float thinkTime=30) :
            liveTime(liveTime), eatTime(eatTime), thinkTime(thinkTime) {}
        float liveTime;
        float eatTime;
        float thinkTime;
    };

private:
    Config config;
    std::vector<PhilosopherInterface*> philosophers;
    std::vector<std::string> logs;
    void pushLog(std::string str);

    //blocks[id] means that philosopher id is bloked.
    //if bit 0 (blocks[id]  & 0x1) then next bloking
    //if bit 1 (blocks[id]  & 0x2) then previous bloking
    std::vector<int> blocks;


    /**
     * @brief allowFor
     * @return
     */
    bool checkIfNeighboursEating(int philosopherId);
    void checkIfNeighboursWaiting(int philosopherId);
    void allowEatingAll();
    void allowEating(int philosopherId);
    void neighbours(int philosopherId, int& previousIndex, int& nextIndex);

    //fifo queue to avoid process starvation
    std::vector<unsigned int> queue;

public:

    Waiter(float liveTime=60, float eatTime=20, float thinkTime=30);

    void setConfig(float liveTime, float eatTime, float thinkTime);

    bool workingStatus() const {return working;}
    void setWorkingStatus(int w){working = w;}

    const Config& getConfig() const {return config;}
    void setPhilosophers(const std::vector<PhilosopherInterface*>& philosophers);


    /**
     * Under the mutex logsMutex
     */
    void askWaiter(int philosopherId);
    std::vector<std::string> getLogs();
    void changedPhilosopherState(int id, int newState);

};

#endif // WAITER_H
