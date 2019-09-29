#ifndef DRAFT_2_RESULT_TYPE_H
#define DRAFT_2_RESULT_TYPE_H

#include <vector>
#include "Cell.h"

using namespace std;

template<typename T>
class Result_Type {
public:
    T total_cost;
    vector<Cell> m_path;
    vector<vector<bool>> m_explored;
    double exec_time;
    long long expanded_cells_count;
    Result_Type() {
        total_cost = -1;
        exec_time = -1;
    }
    Result_Type(T total_cost,
                vector<Cell> path,
                vector<vector<bool>> explored,
                double exec_time,
                long long cells_expanded) :
                total_cost(total_cost), exec_time(exec_time), expanded_cells_count(cells_expanded) {
        m_path = path;
        m_explored = explored;
    }
};


#endif //DRAFT_2_RESULT_TYPE_H
