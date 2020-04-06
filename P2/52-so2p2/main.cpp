#include <iostream>
#include <ctime>
#include "MainWindow.h"

using namespace std;


int main(int argc, char*argv[]){

    srand(time(NULL));


    Config c;

    if(argc >= 2) c.philosofersSize = atoi(argv[1]);
    if(argc >= 3) c.eatTime = (float)atoi(argv[2]);
    if(argc >= 4) c.thinkTime = (float)atoi(argv[3]);
    if(argc >= 5) c.liveTime = (float)atoi(argv[4]);


    MainWindow m(c);
    return m.run();
}
