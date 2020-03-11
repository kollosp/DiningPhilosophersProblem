#include "Philosopher.h"

void Philosopher::initTimes()
{
    leftLiveTime = waiter.getConfig().liveTime;
    leftThinkTime = rand()%(int)waiter.getConfig().thinkTime;
    leftEatTime = rand()%(int)waiter.getConfig().eatTime;
}

Philosopher::Philosopher(Waiter &w, int id) :
    waiter(w), id(id)
{
    value = 0;
    state = 0;
    initTimes();
}

Philosopher::~Philosopher()
{

}


void Philosopher::run()
{
    MyTimer tm;
    tm.update();

    while(waiter.workingStatus()){

        float timeElapsed = tm.timeElapsed()/1000.0;


        leftLiveTime -= timeElapsed;


        /**
          * Process
          * 0 -> 4 -> 3 -> 1 -> 0
          * died in any process set to 2
          */

        //died
        if(leftLiveTime <= 0){
            state = 2;
            waiter.changedPhilosopherState(id, state);
        }

        switch (state) {
        //think
        case 0:
            //end of thinking
            leftThinkTime -= timeElapsed;
            if(leftThinkTime  < 0){
                state = 3;
                waiter.askWaiter(id);
                waiter.changedPhilosopherState(id, state);
            }
            break;
        //eat
        case 1:
            leftEatTime -= timeElapsed;
            if(leftEatTime< 0){
                state = 0;
                waiter.changedPhilosopherState(id, state);
                //reinit times
                initTimes();
            }
            break;
        case 2:
            return;
            break;
        //waiting for eating
        case 3:

            //waiting for call startEating by waiter
            break;
        default:
            break;
        }

        tm.update();
        std::this_thread::sleep_for (std::chrono::milliseconds(20));
    }
}

void Philosopher::startEating()
{
    if(state == 3){
        state = 1;
    }
}
