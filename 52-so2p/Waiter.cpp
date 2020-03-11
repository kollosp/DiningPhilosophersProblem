#include "Waiter.h"

void Waiter::pushLog(std::string str)
{
    if(logs.size()>5){
        logs.erase(logs.begin());
    }
    logs.push_back(str);
}

void Waiter::setPhilosophers(const std::vector<PhilosopherInterface *> &philosophers) {
    this->philosophers = philosophers;

    blocks = std::vector<int>();
    for(int i=0;i<philosophers.size();++i){
        blocks.push_back(0);
    }
}

bool Waiter::checkIfNeighboursEating(int philosopherId)
{
    int prevoiusIndex = philosopherId -1;
    if(prevoiusIndex < 0)
        prevoiusIndex += philosophers.size();

    int nextIndex = philosopherId + 1;
    if(nextIndex >= philosophers.size())
        nextIndex = 0;


    PhilosopherInterface* prevoius = philosophers[prevoiusIndex];
    PhilosopherInterface* next= philosophers[nextIndex];

    bool allowed = true;
    if(prevoius->getState() == 1){
        allowed = false;
        blocks[philosopherId] = blocks[philosopherId] | 0x2;
    }

    if(next->getState() == 1){
        allowed = false;
        blocks[philosopherId] = blocks[philosopherId] | 0x1;
    }

    return allowed;
}

void Waiter::checkIfNeighboursWaiting(int philosopherId)
{
    //unlock neighbours
    int p, n;
    neighbours(philosopherId, p, n);
    //unset blocks
    blocks[p] = blocks[p] & (~0x1);
    blocks[n] = blocks[n] & (~0x2);

    allowEatingAll();

}

void Waiter::allowEatingAll()
{
    std::vector<unsigned int> newQueue;
    for(int i=0;i<queue.size();++i){
        if(blocks[queue[i]] == 0){
            allowEating(queue[i]);
        }
        else{
            newQueue.push_back(queue[i]);
        }
    }

    queue = newQueue;
}

void Waiter::allowEating(int philosopherId)
{
    std::stringstream ss;
    ss<<"ph("<<philosopherId+1<<") eating";
    blocks[philosopherId] = 0;

    //block neighbours
    int p, n;
    neighbours(philosopherId, p, n);
    //set blocks
    blocks[p] = blocks[p] | 0x1;
    blocks[n] = blocks[n] | 0x2;

    philosophers[philosopherId]->startEating();
    pushLog(ss.str());
}

void Waiter::neighbours(int philosopherId, int &previousIndex, int &nextIndex)
{
    previousIndex = philosopherId -1;
    if(previousIndex < 0)
        previousIndex += philosophers.size();

    nextIndex = philosopherId + 1;
    if(nextIndex >= philosophers.size())
        nextIndex = 0;

}

Waiter::Waiter(float liveTime, float eatTime, float thinkTime) :
    working(true),
    config(liveTime, eatTime, thinkTime)
{

}

void Waiter::setConfig(float liveTime, float eatTime, float thinkTime)
{
    config = Config(liveTime, eatTime, thinkTime);
}

void Waiter::askWaiter(int philosopherId)
{
    std::lock_guard<std::mutex> guard(logsMutex);

    std::stringstream ss;
    ss<<"ph("<<philosopherId<<") addes to queue";
    pushLog(ss.str());
    queue.push_back(philosopherId);
    allowEatingAll();
}

std::vector<std::string> Waiter::getLogs() {

    std::lock_guard<std::mutex> guard(logsMutex);
    return logs;
}

void Waiter::changedPhilosopherState(int id, int newState)
{
    std::lock_guard<std::mutex> guard(logsMutex);
    std::stringstream ss;
    ss<<"ph("<<id+1<<") ";

    switch(newState){
    case 0: ss<<"thinking"; break;
    case 1: ss<<"eating"; break;
    case 2: ss<<"died"; break;
    case 3: ss<<"waiting "<<blocks[id]; break;
    default: ss<<"get undefined"; break;
    }

    pushLog(ss.str());

    //finish eating or died
    if(newState == 0 || newState == 2){

        checkIfNeighboursWaiting(id);

    }

}
