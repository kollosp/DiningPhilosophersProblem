#ifndef BOARD_H
#define BOARD_H

#include <vector>
#include <mutex>
#include <stdlib.h>


#include <ncurses.h>
#include <thread>
#include "constants.h"

class Board
{
public:
    class Field
    {
        bool isWall = false;
        int points = 1;
        //0 std point, 1 super point, 2 heal point, 3 big point
        int pointType = 0;
    public:
        Field() {}
        void setWall() {isWall = true; points=0;}
        bool checkIfWall() {return isWall;}
        bool subtructPoint();
        int getPoints() {return points;}
        int getPointType() {return pointType;}
        void setSuperPoint(int _pointType){isWall = false; points=1; pointType=_pointType;}
    };

private:

    std::vector<std::vector<Field> > fields;

    void buildRightSite();
    void copyRightSite();
    void calculatePoints();
    void init(int width, int heigth);

    std::mutex boardAccess;

    int points;

public:
    Board();

    unsigned int getHeight(){return fields.size();}
    unsigned int getWidth();

    void build(int width, int heigth);
    Field& get(unsigned int y, unsigned int x);

    bool checkMove(unsigned int y, unsigned int x);
    bool notifyPlayerBeing(unsigned int y, unsigned int x);
    int getPoints();

    void draw(WINDOW *wn, int offsetX=0, int offsetY=0);
};

#endif // BOARD_H
