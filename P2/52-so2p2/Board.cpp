#include "Board.h"

void Board::buildRightSite()
{
    for(unsigned int j=0;j<fields.size();++j){
        for(unsigned int i=0;i<fields[j].size()/2;++i){

            if(j == 0 || i==0 || j == fields.size()-1){
                fields[j][i].setWall();
            }else if(rand()%10 == 0){
                fields[j][i].setWall();
            }else{
                //randomise point type (stop mobs)

                if(rand()%100 > 40){
                    fields[j][i].setSuperPoint(3);
                }
                else if(rand()%40 == 0){
                    fields[j][i].setSuperPoint(1);
                }
                //heal point
                else if(rand()%60 == 0){
                    fields[j][i].setSuperPoint(2);
                }
            }
        }
    }
}

void Board::copyRightSite()
{
    for(unsigned int j=0;j<fields.size();++j){

        unsigned int x0 = fields[0].size()/2;
        for(unsigned int i=x0;i<fields[0].size();++i){

            fields[j][i] = fields[j][fields[0].size()-i-1];
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
    calculatePoints();
}

Board::Field &Board::get(unsigned int y, unsigned int x)
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

bool Board::notifyPlayerBeing(unsigned int y, unsigned int x)
{
    //field is active since points != 0 when player stands over
    if(fields[y][x].getPoints() > 0){
        points--;
        fields[y][x].subtructPoint();
        return true;
    }

    return false;
}

int Board::getPoints()
{
    return points;
}

void Board::draw(WINDOW *wn, int offsetX, int offsetY)
{
    for(unsigned int y=0;y<getHeight();++y){

        for(unsigned int x=0;x<getWidth();++x){
            if(get(y,x).checkIfWall()){

                wattron(wn,COLOR_PAIR(WALL_PAIR));
                mvwaddstr(wn,y+offsetY,x+offsetX,WALL_CHAR);
                wattroff(wn, COLOR_PAIR(WALL_PAIR));

            }else{
                if(get(y,x).getPoints() > 0){

                    switch(get(y,x).getPointType()){
                    case 0:
                        wattron(wn, COLOR_PAIR(POINT_PAIR));
                        mvwaddstr(wn,y+offsetY,x+offsetX,POINT_CHAR);
                        wattroff(wn, COLOR_PAIR(POINT_PAIR));
                        break;
                    case 1:
                        wattron(wn, COLOR_PAIR(SUPER_POINT_PAIR));
                        mvwaddstr(wn,y+offsetY,x+offsetX,SUPER_POINT_CHAR);
                        wattroff(wn, COLOR_PAIR(SUPER_POINT_PAIR));
                        break;
                    case 2:
                        wattron(wn, COLOR_PAIR(SUPER_POINT_2_PAIR));
                        mvwaddstr(wn,y+offsetY,x+offsetX,SUPER_POINT_2_CHAR);
                        wattroff(wn, COLOR_PAIR(SUPER_POINT_2_PAIR));
                        break;
                    case 3:
                        wattron(wn, COLOR_PAIR(POINT_PAIR));
                        mvwaddstr(wn,y+offsetY,x+offsetX,BIG_POINT_CHAR);
                        wattroff(wn, COLOR_PAIR(POINT_PAIR));
                        break;
                    }


                }else{
                    wattron(wn, COLOR_PAIR(EMPTY_PAIR));
                    mvwaddstr(wn,y+offsetY,x+offsetX,EMPTY_CHAR);
                    wattroff(wn, COLOR_PAIR(EMPTY_PAIR));
                }
            }
        }
    }
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
