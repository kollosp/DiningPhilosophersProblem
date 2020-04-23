#ifndef MYTIME_H
#define MYTIME_H

#include <sys/time.h>
#include <iostream>
#include <unistd.h>
#include <stdio.h>
#include <chrono>

using namespace std;

class MyTimer
{
private:
    std::chrono::steady_clock::time_point c_lastTm;

public:
    MyTimer();

    bool isTimeExp(int m_iMilliSecs);
    /*funkcja sprawdza czy od czasu c_lastTm uplynela odpowiednia ilosc milisekund
     * m_iMilliSecs = ilosc milisekund jaka miala uplynac od ostatniego wywolania funkcji update
     *@return = 1: jesli minelo wiecej czasu niz m_iMinlliSecs
     *          0: jesli czas nie uplynal
     */

    float timeElapsed();
    /*funkcja sprawdza czy od czasu c_lastTm uplynela odpowiednia ilosc milisekund
     * m_iMilliSecs = ilosc milisekund jaka miala uplynac od ostatniego wywolania funkcji update
     *@return = 1: jesli minelo wiecej czasu niz m_iMinlliSecs
     *          0: jesli czas nie uplynal
     */

    void update();
    /*Funkcja sprawdza aktualny czas i ustawia go jako c_lastTime
     */

};

#endif // MYTIME_H
