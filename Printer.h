//
// Created by ipakhalko on 9/8/19.
//

#ifndef DRAFT_2_PRINTER_H
#define DRAFT_2_PRINTER_H

#include <fstream>
#include "Input_Type.h"
#include "Result_Type.h"

using namespace std;

ofstream out("output.txt");

template<typename T>
class Printer {
public:
    static const void print_main_info(const Input_Type<T>& input, const Result_Type<T>& res) {
        if (input.h_key() != "0") {
            out << "A* algorithm - \n";
        } else {
            out << "Dijkstra algorithm - \n";
        }
        out << "path cost: " << res.total_cost << "\n";
        out << "time taken: " << res.exec_time << endl;
        out << endl;
    }

    static const void print_map(const Input_Type<T>& input, const Result_Type<T>& res) {
        vector<vector<char>> cell_status(input.get_field_size().first, vector<char>(input.get_field_size().second, '.'));
        for (int i = 0; i < res.m_explored.size(); ++i) {
            for (int j = 0; j < res.m_explored.back().size(); ++j) {
                if (res.m_explored[i][j]) {
                    cell_status[i][j] = '\'';
                } else if (input.get_weight(i, j) == -1) {
                    cell_status[i][j] = '#';
                }
            }
        }
        for (const auto& elem : res.m_path) {
            cell_status[elem.x][elem.y] = 'o';
        }
        cell_status[input.get_start().x][input.get_start().y] = 'S';
        cell_status[input.get_finish().x][input.get_finish().y] = 'F';

        for (int i = 0; i < res.m_explored.size(); ++i) {
            for (int j = 0; j < res.m_explored.back().size(); ++j) {
                out << cell_status[i][j] << " ";
            }
            out << endl;
        }
    }
};


#endif //DRAFT_2_PRINTER_H
