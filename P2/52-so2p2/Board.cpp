#include "Board.h"

Board::Board()
{

}

void Board::init(int width, heigth)
{
    //board initialization
    fields = std::vector<std::vector<Field>>();

    for(int j=0;j<heigth;++j){

        std::vector<Field> row;

        for(int i=0;i<width;++i){
            row.push_back(fields);
        }

        fields.push_back(row);
    }
}
