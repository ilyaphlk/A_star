//
// Created by ipakhalko on 9/6/19.
//

#ifndef DRAFT_2_GRAPH_H
#define DRAFT_2_GRAPH_H

#include <vector>
#include "Cell.h"
#include "Input_Type.h"

using namespace std;

template <typename T, typename container_type>
class Graph {
    container_type contents;
public:
    Graph() = default;
    Graph(Input_Type<T> input) {
        contents = input.get_weights();
    }
    vector<Cell> get_neighbours(const Cell& c) {
        vector<Cell> neighbours;
        if (c.x > 0 && contents[c.x - 1][c.y] != -1) {
            neighbours.push_back(Cell(c.x - 1, c.y));
        }
        if (c.y > 0 && contents[c.x][c.y - 1] != -1) {
            neighbours.push_back(Cell(c.x, c.y - 1));
        }
        if (c.x < contents.size() - 1 && contents[c.x + 1][c.y] != -1) {
            neighbours.push_back(Cell(c.x + 1, c.y));
        }
        if (c.y < contents[c.x].size() - 1 && contents[c.x][c.y + 1] != -1) {
            neighbours.push_back(Cell(c.x, c.y + 1));
        }
        return neighbours;
    }
};


#endif //DRAFT_2_GRAPH_H
