//
// Created by ipakhalko on 9/1/19.
//

#ifndef DRAFT_2_INPUT_TYPE_H
#define DRAFT_2_INPUT_TYPE_H


#include <cstddef>

#include <utility>
#include <vector>
#include <string>
#include <fstream>
#include "Cell.h"
#include "Generator.h"

using namespace std;

template <typename T>
class Input_Type {
    string field_type;
    pair <int, int> field_size; // it is easier that way in terms of subtraction
    Cell start;
    Cell finish;
    vector<vector<T>> weights;
    string heuristic_key;
    string optimization;
    T max_weight;

public:
    Input_Type() = default;
    ~Input_Type() = default;

    void read_input() {
        ifstream in("input.txt");
        in >> field_size.first >> field_size.second;
        weights = vector<vector<T>> (field_size.first, vector<T>(field_size.second, 1));
        in >> start.x >> start.y;
        in >> finish.x >> finish.y;

        string manual_input;
        in >> manual_input;
        if (manual_input == "yes") {
            for (int i = 0; i < field_size.first; ++i) {
                for (int j = 0; j < field_size.second; ++j) {
                    in >> weights[i][j];
                }
            }
        } else {
            in >> max_weight;
            in >> field_type; // snakes, segments, rectangles or rooms
            double congestion_rate;
            in >> congestion_rate;
            Generator<T> gen(field_type, congestion_rate);
            gen.make_field(start, finish, weights, max_weight);
        }
        in >> heuristic_key;
        in >> optimization;
    }

    vector<vector<T>> get_weights() const {
        return weights;
    }

    T get_weight(int i, int j) const {
        return weights[i][j];
    }

    T get_weight(const Cell& c) const {
        return weights[c.x][c.y];
    }

    T get_max_weight() const {
        return max_weight;
    }

    string get_field_type() const {
        return field_type;
    }

    pair<int, int> get_field_size() const {
        return field_size;
    }

    Cell get_start() const {
        return start;
    }

    Cell get_finish() const {
        return finish;
    }

    string h_key() const {
        return heuristic_key;
    }

    void change_h_key(string new_key) {
        heuristic_key = new_key;
    }
};

#endif //DRAFT_2_INPUT_TYPE_H
