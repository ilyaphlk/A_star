//
// Created by ipakhalko on 9/1/19.
//

#ifndef DRAFT_2_CELL_H
#define DRAFT_2_CELL_H

class Cell {
public:
    int x;
    int y;
    Cell() = default;
    Cell(int x, int y) : x(x), y(y) {}
};

bool operator==(const Cell& lhs, const Cell& rhs); /* {
    return lhs.x == rhs.x && lhs.y == rhs.y;
}*/

bool operator!=(const Cell& lhs, const Cell& rhs); /*{
    return !(lhs == rhs);
}*/

bool operator<(const Cell& lhs, const Cell& rhs);/* {
    return lhs.x < rhs.x || (lhs.x == rhs.x && lhs.y < rhs.y);
}*/

#endif //DRAFT_2_CELL_H
