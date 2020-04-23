#ifndef BOARD_H
#define BOARD_H

#include <vector>
#include <mutex>
#include <stdlib.h>

class Board
{
public:
    class Field
    {
        bool isWall = false;
        int points = 1;
    public:
        Field() {}
        void setWall() {isWall = true;}
        bool checkIfWall() {return isWall;}
        bool subtructPoint();
        int getPoints() {return points;}
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
    Field get(unsigned int y, unsigned int x);

    bool checkMove(unsigned int y, unsigned int x);
    void notifyPlayerBeing(unsigned int y, unsigned int x);
    int getPoints();
};

#endif // BOARD_H
