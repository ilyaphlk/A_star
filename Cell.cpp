//
// Created by ipakhalko on 9/1/19.
//

#include "Cell.h"

using namespace std;

bool operator==(const Cell& lhs, const Cell& rhs) {
    return lhs.x == rhs.x && lhs.y == rhs.y;
}

bool operator!=(const Cell& lhs, const Cell& rhs) {
    return !(lhs == rhs);
}

bool operator<(const Cell& lhs, const Cell& rhs) {
    return lhs.x < rhs.x || (lhs.x == rhs.x && lhs.y < rhs.y);
}