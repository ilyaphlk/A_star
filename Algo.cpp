
//
// Created by ipakhalko on 9/6/19.
//

#include "Algo.h"
#include <string>

double h(const Cell& current, const Cell& start, const Cell& goal, string h_type) {
    if (h_type == "0") {
        return 0;
    }
    auto dx1 = current.x - goal.x;
    auto dy1 = current.y - goal.y;
    auto dx2 = start.x - goal.x;
    auto dy2 = start.y - goal.y;
    auto cross_product = abs(dx1*dy2 - dx2*dy1);
    return abs(dx1) + abs(dy1) + cross_product * 1e-3;
}
