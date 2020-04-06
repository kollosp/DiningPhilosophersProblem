#ifndef BOARD_H
#define BOARD_H

#include <vector>

class Board
{
    class Field
    {
    public:
        Field() {}
    };

    std::vector<std::vector<Field> > fields;

public:
    Board();

    init(int width, height);
};

#endif // BOARD_H
