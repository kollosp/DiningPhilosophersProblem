#include "Manager.h"
#include <sstream>

GameManager::GameManager(Board &b):
    board(b)
{}

GameManager::~GameManager()
{
    unlockMobs();
}

std::unique_lock<std::mutex>* GameManager::mobWait()
{
    std::unique_lock<std::mutex>* lk = new std::unique_lock<std::mutex>(mobMutext);
    mobCV.wait((*lk), []{return true;});
    return lk;
}

void GameManager::mobNotify(std::unique_lock<std::mutex>*  lk)
{
    lk->unlock();
    mobCV.notify_one();
}

void GameManager::lockMobs()
{
    //lock all mobs
    tm.update();
    if(mobsLocked  == false)
        lockMobsLck = mobWait();
    mobsLocked = true;
}

void GameManager::lock()
{
    std::lock_guard<std::mutex> guard(mobMutext);
}

void GameManager::unlock()
{

}

int GameManager::manage()
{
    std::lock_guard<std::mutex> guard(checkPositionMutex);

    //try to unlock mobs
    if(mobsLocked){
        if(tm.isTimeExp(5000)){
            unlockMobs();
        }
    }

    if(superPlayer){
        if(superPlayerTimer.isTimeExp(5000)){
            superPlayer = false;
        }
    }

    //std::lock_guard<std::mutex> guard(toDoInMainMutex);

    int ret = 0;
    if(toDoInMain){
        ret = toDoInMain;
    }

    toDoInMain = 0;
    return ret;

}

void GameManager::pushBackMob(PlayerInterface *p)
{
    mobs.push_back(p);
    allCreatures.push_back(p);
}

void GameManager::setPlayer(PlayerInterface *p)
{
    player = p;
    allCreatures.push_back(p);
}

int GameManager::notifyPlayerBeing(PlayerInterface* p, unsigned int y, unsigned int x)
{

    //std::lock_guard<std::mutex> guard(checkPositionMutex);

    if(p == player){
        if(board.notifyPlayerBeing(y,x)){
            //super point
            switch(board.get(y,x).getPointType()){
            //super point 1 - stop mobs
            case 1:
                lockMobs();
                break;
            case 2:
                //heal
                player->getDamage(-1);
                break;
            case 3:
                superPlayerTimer.update();
                superPlayer = true;
                break;
            }
        }
    }else if(hunters[0] == p){
        //mob eat super field - hunt finished
        Board::Field f = board.get(y,x);
        if(f.getPoints() > 0 && f.getPointType() > 0){
            board.notifyPlayerBeing(y,x);
            f.setSuperPoint(0);

            //spawn new mob
            if(allCreatures.size() < 40)
                toDoInMain = 1;

            //remove actual mob
            if(hunters.size() > 0){
                hunters.erase(hunters.begin());
            }

            //allow next
            if(hunters.size() > 0){
                hunters[0]->notifyKeyPressed(0);
            }

            return 1;
        }
    }

    return 0;
}

bool GameManager::checkMove(PlayerInterface *p, unsigned int y, unsigned int x)
{
    //std::lock_guard<std::mutex> guard(checkPositionMutex);

    if(board.checkMove(y,x) == false){
        return false;
    }

    for(int i=0;i<allCreatures.size();++i){
        if(p != allCreatures[i]){
            //.. check if the same position
            if(p->getX() == allCreatures[i]->getX() && p->getY() == allCreatures[i]->getY()){
                //collision detected

                if(p == player || allCreatures[i] == player){
                    //kill mob
                    if(superPlayer){
                        //PlayerInterface* pp = (p == player ? allCreatures[i] : p);
                        //pp->getDamage(1);
                    }
                    //kill player
                    else{
                        player->spawn();
                        player->getDamage(1);

                        toDoInMain = 1;
                    }

                    //player gets damage
                    return false;
                }else{
                    //mobs collision = nothink to do
                    return false;
                }
            }
        }
    }

    //no collision detected
    return true;
}

void GameManager::unlockMobs()
{
    if(mobsLocked ){
        mobsLocked = false;
        mobNotify(lockMobsLck);

        for(unsigned int i=0;i<mobs.size();++i)
            mobs[i]->clearClock();
    }
}

void GameManager::askForHunt(PlayerInterface* p)
{
    std::lock_guard<std::mutex> guard(checkPositionMutex);

    if(hunters.size() == 0){
        //notify mob can starts hunting
        p->notifyKeyPressed(0);
    }

    hunters.push_back(p);
}

std::string GameManager::getHunters()
{
    std::stringstream ss;
    for(unsigned int i=0;i<hunters.size();++i){
        if(i == 14){
            ss<<"..";
            break;
        }
        ss << (char)(hunters[i]->getId() + '0' -1);
        ss << ">";
    }

    std::string s = ss.str();

    while(s.size() < 15)
        s+= " ";

    return s;
}

void GameManager::notifyPlayerDied(PlayerInterface *p)
{
    for(unsigned int i=0;i<hunters.size();++i){
        if(hunters[i] == p){
            hunters.erase(hunters.begin()+i);
            if(i == 0 && hunters.size() > 0){
                hunters[0]->notifyKeyPressed(0);
            }
            break;
        }
    }
}
