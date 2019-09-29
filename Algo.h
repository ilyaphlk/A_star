//
// Created by ipakhalko on 9/6/19.
//

#ifndef DRAFT_2_ALGO_H
#define DRAFT_2_ALGO_H

#include <ctime>
#include <set>
#include <algorithm>
#include "Result_Type.h"
#include "Input_Type.h"
#include "Graph.h"

using namespace std;

template <typename T>
class estimates {
public:
    double f;
    T g;
    estimates(){}
    estimates(double f, T g) : f(f), g(g) {}
};

double h(const Cell& current, const Cell& start, const Cell& goal, string h_type);

template <typename T>
class Algo {
public:
    static Result_Type<T> A_star(Input_Type<T> input) {
        const int INF = 2e9;
        std::clock_t c_start = std::clock();

        //build Graph object
        Graph<T, vector<vector<T>>> g(input);

        //make other magic
        auto start = input.get_start();
        auto finish = input.get_finish();
        if (input.get_weight(start) == -1 || input.get_weight(finish) == -1) {
            return Result_Type<T>();
        }


        int n = input.get_field_size().first, m = input.get_field_size().second;
        set<pair<double, Cell>> f_queue;
        vector<vector<bool>> used(n, vector<bool>(m));
        vector<vector<Cell>> parent(n, vector<Cell>(m));
        vector<vector<estimates<T>>> estimatesMatrix(n, vector<estimates<T>>(m, estimates<T>(INF, INF)));

        f_queue.insert({input.get_weight(start.x, start.y), start});
        estimatesMatrix[start.x][start.y] = estimates<T>(
                input.get_weight(start.x, start.y),
                input.get_weight(start.x, start.y));

        long long expanded_cells_count = 0;
        while (!f_queue.empty()) {
            auto v = f_queue.begin()->second;
            f_queue.erase(f_queue.begin());

            if (v == finish) {
                used[finish.x][finish.y] = true;
                ++expanded_cells_count;
                std::clock_t c_end = std::clock();

                vector <Cell> path;
                Cell current = finish;
                while (current != start) {
                    path.push_back(current);
                    current = parent[current.x][current.y];
                }
                path.push_back(start);

                reverse(path.begin(), path.end());
                return Result_Type<T>(estimatesMatrix[finish.x][finish.y].g,
                                      path, used, 1000.0 * (c_end-c_start) / CLOCKS_PER_SEC, expanded_cells_count);
            }

            if (!used[v.x][v.y]) {
                ++expanded_cells_count;
            }
            used[v.x][v.y] = true;
            for (const Cell& u : g.get_neighbours(v)) {
                auto relaxedCost = estimatesMatrix[v.x][v.y].g + input.get_weight(u.x, u.y);
                if (relaxedCost < estimatesMatrix[u.x][u.y].g) {
                    parent[u.x][u.y] = v;
                    estimatesMatrix[u.x][u.y].g = relaxedCost;
                }

                auto relaxedF = estimatesMatrix[u.x][u.y].g + h(u, start, finish, input.h_key());
                if (!used[u.x][u.y] || relaxedF < estimatesMatrix[u.x][u.y].f) {
                    estimatesMatrix[u.x][u.y].f = relaxedF;
                    f_queue.insert({relaxedF, u});
                    if (used[u.x][u.y]) {
                        --expanded_cells_count;
                    }
                    used[u.x][u.y] = false;
                }
            }
        }

        std::clock_t c_end = std::clock();
        return Result_Type<T>(-1, {}, used, 1000.0 * (c_end-c_start) / CLOCKS_PER_SEC, expanded_cells_count);
    }

    static Result_Type<T> Dijkstra(Input_Type<T> input) {
        input.change_h_key("0");
        return A_star(input);
    }
};


#endif //DRAFT_2_ALGO_H
