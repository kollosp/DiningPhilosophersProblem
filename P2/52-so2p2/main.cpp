#include <iostream>
#include <ctime>
#include "MainWindow.h"

using namespace std;


int main(int argc, char*argv[]){

    srand(time(NULL));
    MainWindow m;
    return m.run();
}
