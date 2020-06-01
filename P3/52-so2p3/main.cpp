#include <iostream>
#include <vector>
#include <thread>
#include <mutex>

#include <sstream>
#include <iomanip>
#include <ncurses.h>

#include <condition_variable>

#include "MyTime.h"

using namespace std;

const unsigned int RESOURCES_FLOUR_MAX_SIZE = 10;
const unsigned int RESOURCES_BURGER_MAX_SIZE = 10;

const unsigned int TIME_CLIENT_WAIT_FOR_ORDER_MIN = 5000;
const unsigned int TIME_CLIENT_WAIT_FOR_ORDER_MAX = 20000;

const unsigned int TIME_CLIENT_EAT_MIN = 5000;
const unsigned int TIME_CLIENT_EAT_MAX = 20000;

const unsigned int TIME_SUPPORT_WAIT_WAREHOUSE_MIN= 5000;
const unsigned int TIME_SUPPORT_WAIT_WAREHOUSE_MAX= 20000;

const unsigned int TIME_SUPPORT_WAIT_BETWEEN_DELIVERY_MIN= 5000;
const unsigned int TIME_SUPPORT_WAIT_BETWEEN_DELIVERY_MAX= 20000;

const unsigned int TIME_COOKER_COOK_MIN = 5000;
const unsigned int TIME_COOKER_COOK_MAX = 10000;

const int consoleW = 80;
const int consoleH = 24;
const int winposX = 0;
const int winposY = 0;

int WORK = true;


int ORDERS = 0;
mutex ordersMutex;
condition_variable orderCondVar;

vector<thread*> threads;

class Resources_Flour{

};

class Resources_Burger{

};

class Client{
public:
    int state;
    int elapsed;
    int timeToDie;
    int timeBetweenOrders;

    int getTime(){
        switch (state) {
        case 0: return timeToDie;
        case 1: return timeBetweenOrders;
        case 2: return -1;
        }
        return 0;
    }

    string getState(){
        switch (state) {
        case 0: return "oczekiwanie";
        case 1: return "jedzenie   ";
        case 2: return "rezygnacja ";
        }
        return "undef";
    }
};

class Cooker{
public:
    int state;
    int elapsed;
    int burgerCreationTime;


    int getTime(){
        switch (state) {
        case 0: return -1;
        case 1: return burgerCreationTime;
        }
        return 0;
    }

    string getState(){
        switch (state) {
        case 0: return "oczekiwanie";
        case 1: return "produkcja  ";
        }
        return "undef";
    }
};

class Support{
public:
    int state;
    int elapsed;
    int timeMax;
    int timeBetweenSupplies;
    int timeOnDelivery;

    int getTime(){
        switch (state) {
        case 0: return timeBetweenSupplies;
        case 1: return timeOnDelivery;
        }
        return 0;
    }

    string getState(){
        switch (state) {
        case 0: return "dostawa     ";
        case 1: return "oczekiwanie ";
        }
        return "undef";
    }
};

vector<Client> clients;
vector<Cooker> cookers;
vector<Support> supports;


//flour resources and mutex
vector<Resources_Flour> flour;
mutex flourMutex;

//burgers resources and mutex
vector<Resources_Burger> burgers;
mutex burgersMutex;

int getChar()
{
    int ch = getch();

    if (ch != ERR) {
        return ch;
    } else {
        return 0;
    }
}

void sleep(){
    std::this_thread::sleep_for (std::chrono::milliseconds(100));
}

void client(unsigned int id){

    //cout<<"Client("<<id<<") hello"<<endl;

    clients[id].timeToDie = rand()%(TIME_CLIENT_WAIT_FOR_ORDER_MAX - TIME_CLIENT_WAIT_FOR_ORDER_MIN) + TIME_CLIENT_WAIT_FOR_ORDER_MIN;
    clients[id].timeBetweenOrders = rand()%(TIME_CLIENT_EAT_MAX-TIME_CLIENT_EAT_MIN) + TIME_CLIENT_EAT_MIN;

    MyTimer tm;
    tm.update();

    //int state = 0; //wait for order
    // state = 1; //eating burger
    // state = 2; //died

    clients[id].state = 0;

    //init thread variables
    while(WORK){

        clients[id].elapsed = tm.timeElapsed();

        switch(clients[id].state){
        //waiting for order
        case 0:
            clients[id].elapsed = tm.timeElapsed();

            //if time to die elapsed then resign from order
            if(tm.isTimeExp(clients[id].timeToDie)){

                lock_guard<mutex> lk(ordersMutex);
                ORDERS --;
                orderCondVar.notify_all();
                tm.update();
                clients[id].state = 2; // resign
                //cout<<"Client("<<id<<") resign from order. bye."<<endl;
                break;
            }

            //lock scope
            {
                lock_guard<mutex> burgersLock(burgersMutex);
                //check if there is any ready order
                if(burgers.size() > 0){
                    burgers.erase(burgers.begin());
                    //cout<<"Client("<<id<<") start eating..."<<endl;
                    //ORDERS --;
                    tm.update(); //update timer
                    clients[id].state = 1; //start eating
                }
            }

            break;

        //eating
        case 1:

            if(tm.isTimeExp(clients[id].timeBetweenOrders)){
                //create new order
                //..

                //cout<<"Client("<<id<<") burger ordered. waiting..."<<endl;
                lock_guard<mutex> burgersLock(burgersMutex);
                lock_guard<mutex> lk(ordersMutex);
                ORDERS ++;
                orderCondVar.notify_all();
                tm.update();
                clients[id].state = 0; // order new burger
                break;
            }

            break;
        case 2:
            clients[id].elapsed = -1;
            break;
        }

        sleep();
    }
}



void supporter(unsigned int id){

    //cout<<"Support("<<id<<") hello"<<endl;

    supports[id].timeOnDelivery = rand()%(TIME_SUPPORT_WAIT_WAREHOUSE_MAX - TIME_SUPPORT_WAIT_WAREHOUSE_MIN)
            + TIME_SUPPORT_WAIT_WAREHOUSE_MIN; //20s
    supports[id].timeBetweenSupplies = rand()%(TIME_SUPPORT_WAIT_BETWEEN_DELIVERY_MAX - TIME_SUPPORT_WAIT_BETWEEN_DELIVERY_MIN)
            + TIME_SUPPORT_WAIT_BETWEEN_DELIVERY_MIN; //20s

    //int state = 0; //wait for supply time
    //int state = 1; //wait for warehouse space - if time elapsed then supply lost

    MyTimer tm;
    tm.update();

    supports[id].state = 0;

    //init thread variables
    while(WORK){


        supports[id].elapsed = tm.timeElapsed();

        switch (supports[id].state) {
        case 0:
            if(tm.isTimeExp(supports[id].timeBetweenSupplies)){
                //send new delivery to restaurant
                tm.update();
                supports[id].state = 1;
                //cout<<"Support("<<id<<") new delivery send..."<<endl;
            }
            break;
        case 1:
            if(tm.isTimeExp(supports[id].timeOnDelivery)){
                //can not store delivery in warehouse
                tm.update();
                supports[id].state = 0;
                //cout<<"Support("<<id<<") there is no enough space in warehouse"<<endl;
                break;
            }

            //lock scope
            {
                //check if there is any free space in warehouse
                lock_guard<mutex> flourLock(flourMutex);
                if(flour.size() < RESOURCES_FLOUR_MAX_SIZE){
                    flour.push_back(Resources_Flour());
                    //cout<<"Support("<<id<<") resources stored in warehouse"<<endl;
                    tm.update();
                    supports[id].state = 0;
                }
            }

            break;
        }



        sleep();
    }
}



void cooker(unsigned int id){

    //cout<<"Cooker("<<id<<") hello"<<endl;

    cookers[id].burgerCreationTime = rand()%(TIME_COOKER_COOK_MAX - TIME_COOKER_COOK_MIN) + TIME_COOKER_COOK_MIN;

    cookers[id].state = 0;
    //int state = 0; //wait for clients or resources
    // state = 1; //preparing order (burger)

    MyTimer tm;
    tm.update();

    //init thread variables
    while(WORK){

        cookers[id].elapsed = tm.timeElapsed();

        switch (cookers[id].state) {
        case 0:

            //lock scope
            {
                //check if there is any client

                int f;
                {
                    lock_guard<mutex> flourLock(flourMutex);
                    f = flour.size();
                }

                if(f > 0){
                    std::unique_lock<std::mutex> lk(ordersMutex);
                    orderCondVar.wait(lk, []{return (ORDERS > 0);});

                    flour.erase(flour.begin());
                    cookers[id].state = 1;
                    tm.update();
                    ORDERS --; //order in progress
                    //cout<<"Cooker("<<id<<") cooking..."<<endl;

                    lk.unlock();
                    orderCondVar.notify_one();
                }
            }

            break;
        case 1:
            if(tm.isTimeExp(cookers[id].burgerCreationTime)){

                lock_guard<mutex> burgersLock(burgersMutex);
                if(burgers.size() < RESOURCES_BURGER_MAX_SIZE){
                    cookers[id].state = 0;
                    burgers.push_back(Resources_Burger());
                }

                //cout<<"Cooker("<<id<<") burger cooked"<<endl;

            }
            break;
        default:
            break;
        }


        sleep();
    }
}



string createBar(const string& label, int min, int max, int value, int format = 0, int w=20){
    stringstream ss;
    ss << label<<" ";

    if(max != -1){
        if(format == 1)
            ss<<setw(5)<<value;
        else
            ss<<setw(5)<<(max - value);
        ss<<" [";
    }
    else{
        ss<<"      [";
    }

    float prc = (value - min) / (float)(max - min);
    float h = prc * w;

    int i=0;
    for(;i<h && i<w;++i){
        ss << "#";
    }

    for(;i<w;++i){
        ss << ".";
    }

    ss << "]";
    return ss.str();
}


int main(int argc, char *argv[])
{
    srand(time(NULL));

    if(argc < 4){
        cout<<"Podaj argumenty: <liczba dostawcow> <liczba kucharzy> <liczba klientow>"<<endl;
        return 0;
    }

    unsigned int supportSize = atoi(argv[1]);
    unsigned int cookerSize = atoi(argv[2]);
    unsigned int clientSize = atoi(argv[3]);

    //init
    for(unsigned int i=0;i<supportSize;++i){
        supports.push_back(Support());
        threads.push_back(new std::thread(supporter, i));
    }

    for(unsigned int i=0;i<cookerSize;++i){
        cookers.push_back(Cooker());
        threads.push_back(new std::thread(cooker, i));
    }

    for(unsigned int i=0;i<clientSize;++i){
        clients.push_back(Client());
        threads.push_back(new std::thread(client, i));
    }

    for(int i=0;i<clients.size();++i)
        if(burgers.size() < RESOURCES_BURGER_MAX_SIZE)
            burgers.push_back(Resources_Burger());

    //set orders equal to clients
    //ORDERS = 0; //clientSize;

    initscr();
    timeout(1);
    noecho();

    std::string title = "RESTAURACJA";
    std::string footer = "build: " + std::string(__DATE__) + " " + std::string(__TIME__);

    WINDOW* mainwin = newwin(consoleH,consoleW,winposX,winposY);
    refresh();

    box(mainwin, 0, 0);

    mvwprintw(mainwin, 0, (consoleW-title.size())/2, "%s", title.c_str());
    mvwprintw(mainwin, consoleH-1, (consoleW-footer.size())-1, "%s", footer.c_str());

    wrefresh(mainwin);

    while (WORK) {

        int lineY = 1;
        int lineX = 1;

        //print variables
        lineY++;
        mvwprintw(mainwin, lineY, lineX, "%s", createBar("Zamowienia", 0, clients.size(), ORDERS, 1).c_str());
        lineY++;
        mvwprintw(mainwin, lineY, lineX, "%s", createBar("Maka", 0, 10, flour.size(), 1).c_str());
        lineY++;
        mvwprintw(mainwin, lineY, lineX, "%s", createBar("Burgery", 0, RESOURCES_BURGER_MAX_SIZE, burgers.size(), 1).c_str());

        wrefresh(mainwin);


        //draw progress
        for(int i=0;i<supports.size();++i){
            stringstream ss2;
            ss2<<"Dostawca "<<i<<"."<<supports[i].getState()<<createBar("", 0, supports[i].getTime(), supports[i].elapsed);
            //supports[i].state<<" "<<setw(5)<<supports[i].elapsed<<" | "<<setw(5)<<supports[i].timeOnDelivery<<" "<<setw(5)<<supports[i].timeBetweenSupplies;
            lineY++;
            mvwprintw(mainwin, lineY, lineX, "%s", ss2.str().c_str());
        }

        for(int i=0;i<cookers.size();++i){
            stringstream ss2;
            ss2<<"Kucharz "<<i<<"."<<cookers[i].getState()<<createBar("", 0, cookers[i].getTime(), cookers[i].elapsed);
            lineY++;
            mvwprintw(mainwin, lineY, lineX, "%s", ss2.str().c_str());
        }

        for(int i=0;i<clients.size();++i){
            stringstream ss2;
            ss2<<"Klient "<<i<<". "<<clients[i].getState()<<" "<<setw(6)<<createBar("", 0, clients[i].getTime(), clients[i].elapsed);
            lineY++;
            mvwprintw(mainwin, lineY, lineX, "%s", ss2.str().c_str());
        }

        int c = getChar();

        if(c == 'q'){
            //close procedure
            WORK = false;
            //unlock all locked cookers
            ORDERS = cookers.size();
            orderCondVar.notify_all();
        }

        sleep();
    }



    //join all threads
    for(unsigned int i=0;i<threads.size();++i){
        threads[i]->join();
    }

    //destroy all threads
    for(unsigned int i=0;i<threads.size();++i){
        delete threads[i];
    }

    endwin();

    return 0;
}
