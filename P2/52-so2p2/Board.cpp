#include "Board.h"

void Board::buildRightSite()
{
    for(unsigned int j=0;j<fields.size();++j){
        for(unsigned int i=0;i<fields[j].size()/2;++i){
            if(rand()%10 == 0){
                fields[j][i].setWall();
            }
        }
    }

}

void Board::copyRightSite()
{

    for(unsigned int j=0;j<fields.size();++j){

        unsigned int x0 = fields[0].size()/2;
        for(unsigned int i=x0;i<fields[0].size();++i){

            fields[j][i] = fields[j][fields[0].size()-i];
        }
    }
}

void Board::calculatePoints()
{
    points = 0;
    for(unsigned int j=0;j<fields.size();++j){
        for (unsigned int i=0;i<fields[j].size();++i){
            points += fields[j][i].getPoints();
        }
    }
}

Board::Board()
{

}

unsigned int Board::getWidth()
{
    if(fields.size() > 0){
        return fields[0].size();
    }

    return 0;
}

void Board::build(int width, int heigth)
{
    init(width, heigth);
    buildRightSite();
    copyRightSite();
}

Board::Field Board::get(unsigned int y, unsigned int x)
{
    return fields[y][x];
}

bool Board::checkMove(unsigned int y, unsigned int x)
{
    std::lock_guard<std::mutex> guard(boardAccess);

    if(x < 0 || x >= getWidth()) return false;
    if(y < 0 || y >= getHeight()) return false;

    return !fields[y][x].checkIfWall();
}

void Board::notifyPlayerBeing(unsigned int y, unsigned int x)
{
    if(fields[y][x].getPoints() > 0)
        points--;

    fields[y][x].subtructPoint();
}

int Board::getPoints()
{
    return points;
}

void Board::init(int width, int heigth)
{
    //board initialization
    fields = std::vector<std::vector<Field>>();

    for(int j=0;j<heigth;++j){

        std::vector<Field> row;

        for(int i=0;i<width;++i){
            row.push_back(Field());
        }

        fields.push_back(row);
    }
}

bool Board::Field::subtructPoint()
{
    if(points >= 1)
        points=-1;

    return points;
}
