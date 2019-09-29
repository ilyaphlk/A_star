#include <utility>

//
// Created by ipakhalko on 9/1/19.
//

#ifndef DRAFT_2_GENERATOR_H
#define DRAFT_2_GENERATOR_H

#include <set>
#include <string>
#include <vector>
#include "Cell.h"

using namespace std;

template <typename T>
void adjust_directions_choice(set<string>& available_directions,
                              long long cur_x, long long cur_y,
                              long long prev_x, long long prev_y,
                              vector<vector<T>>& weights) {
    if (cur_x - prev_x == 1) {
        available_directions.erase("up");
    } else if (cur_x - prev_x == -1) {
        available_directions.erase("down");
    } else if (cur_y - prev_y == 1) {
        available_directions.erase("right");
    } else if (cur_y - prev_y == -1) {
        available_directions.erase("left");
    }

    if (cur_x == weights.size() - 1) {
        available_directions.erase("down");
    }
    if (cur_x == 0) {
        available_directions.erase("up");
    }
    if (cur_y == weights.back().size() - 1) {
        available_directions.erase("right");
    }
    if (cur_y == 0) {
        available_directions.erase("left");
    }
}

template <typename T>
void make_snakes(long long obstacle_count, vector<vector<T>>& weights, const Cell& start, const Cell& finish) {
    while (obstacle_count > 0) {
        long long snake_length = max(obstacle_count * 0.01, 1.);

        int snake_start_x, snake_start_y;
        do {
            snake_start_x = rand() % weights.size();
            snake_start_y = rand() % weights.back().size();
        } while (snake_start_x == start.x && snake_start_y == start.y);

        auto cur_x = snake_start_x, cur_y = snake_start_y;
        auto prev_x = cur_x, prev_y = cur_y;
        auto length_left = snake_length;
        long long segment_size_left = 0;
        while (length_left > 0) {
            weights[cur_x][cur_y] = -1;
            --length_left;

            if (cur_x + (cur_x - prev_x) >= 0 && cur_x + (cur_x - prev_x) < weights.size() &&
                cur_y + (cur_y - prev_y) >= 0 && cur_y + (cur_y - prev_y) < weights.back().size() &&
                segment_size_left > 0) {
                auto tmp = prev_x;
                prev_x = cur_x;
                cur_x = cur_x + (cur_x - tmp);

                tmp = prev_y;
                prev_y = cur_y;
                cur_y = cur_y + (cur_y - tmp);
                --segment_size_left;
            } else {
                segment_size_left = max((long long)((10 + rand() % 30) / 100. * length_left), 1LL);
                set<string> available_directions = {"up", "down", "left", "right"};
                adjust_directions_choice(available_directions, cur_x, cur_y,
                                         prev_x, prev_y, weights);

                int delta = rand() % available_directions.size();
                auto it = available_directions.begin();
                while (delta > 0) {
                    ++it;
                    --delta;
                }
                string new_direction = *it;

                prev_x = cur_x, prev_y = cur_y;
                if (new_direction == "up") {
                    cur_x = prev_x - 1;
                } else if (new_direction == "down") {
                    cur_x = prev_x + 1;
                } else if (new_direction == "left") {
                    cur_y = prev_y - 1;
                } else if (new_direction == "right") {
                    cur_y = prev_y + 1;
                }
            }
        }
        obstacle_count -= snake_length;
    }

    weights[start.x][start.y] = max(1, weights[start.x][start.y]);
    weights[finish.x][finish.y] = max(1, weights[finish.x][finish.y]);
}

template <typename T>
void make_rooms(long long obstacle_count, vector<vector<T>>& weights, const Cell& start, const Cell& finish) {
    double horizontal_walls_share = ((rand() % 20) + 40) / 100.;
    long long horizontal_walls_count = (obstacle_count * horizontal_walls_share) / weights.back().size();
    long long vertical_walls_count = (obstacle_count * (1 - horizontal_walls_share)) / weights.size();

    vector<int> x_coords;
    int prev_x = 0;
    while (horizontal_walls_count > 0) {
        int cur_x = prev_x + rand() % (max((long long)weights.size() - horizontal_walls_count - prev_x, 1LL));
        if (cur_x < weights.size()) {
            x_coords.push_back(cur_x);
            for (int i = 0; i < weights.back().size(); ++i) {
                weights[cur_x][i] = -1;
            }
            prev_x = cur_x;
        }
        --horizontal_walls_count;
    }

    vector<int> y_coords;
    int prev_y = 0;
    while (vertical_walls_count > 0) {
        int cur_y = prev_y + rand() % (max((long long)weights.back().size() - vertical_walls_count - prev_y, 1LL));
        if (cur_y < weights.back().size()) {
            y_coords.push_back(cur_y);
            for (int i = 0; i < weights.size(); ++i) {
                weights[i][cur_y] = -1;
            }
            prev_y = cur_y;
        }
        --vertical_walls_count;
    }

    prev_x = 0;
    for (int i = 0; i < x_coords.size(); ++i) {
        auto cur_x = x_coords[i];
        prev_y = 0;
        for (int j = 0; j < y_coords.size(); ++j) {
            auto cur_y = y_coords[j]; /// double check this being sorted

            int vertical_segment_length = cur_x - prev_x + 1;
            int vertical_door_length = max(vertical_segment_length * (((rand() % 30) + 10) / 100.), 1.);
            int vertical_door_start = prev_x + rand() % (vertical_segment_length - vertical_door_length + 1);
            for (int delta = 0; delta < vertical_door_length; ++delta) {
                weights[vertical_door_start + delta][cur_y] = 1;
            }

            int horizontal_segment_length = cur_y - prev_y + 1;
            int horizontal_door_length = max(horizontal_segment_length * (((rand() % 30) + 10) / 100.), 1.);
            int horizontal_door_start = prev_y + rand() % (horizontal_segment_length - horizontal_door_length + 1);
            for (int delta = 0; delta < horizontal_door_length; ++delta) {
                weights[cur_x][horizontal_door_start + delta] = 1;
            }

            prev_y = cur_y;
        }
        prev_x = cur_x;
    }

    weights[start.x][start.y] = max(1, weights[start.x][start.y]);
    weights[finish.x][finish.y] = max(1, weights[finish.x][finish.y]);
}

template <typename T>
void make_rectangles(long long obstacle_count, vector<vector<T>>& weights, const Cell& start, const Cell& finish) {
    long long n = weights.size(), m = weights.back().size();
    while (obstacle_count > 0) {
        long long current_area = max(1LL, (long long)(obstacle_count * 0.1));  // upper bound of the actual area
        long long width = max(min(m / 3 + 1, current_area / (rand() % 9 + 1)), 1LL);
        long long height = max(min(n / 3 + 1, current_area / width), 1LL);
        int upperLeftCornerX = rand() % (n - height + 1);
        int upperLeftCornerY = rand() % (m - width + 1);
        if (!(start.x >= upperLeftCornerX && start.x <= upperLeftCornerX + height - 1 &&
              start.y >= upperLeftCornerY && start.y <= upperLeftCornerY + width - 1) &&
              !(finish.x >= upperLeftCornerX && finish.x <= upperLeftCornerX + height - 1 &&
              finish.y >= upperLeftCornerY && finish.y <= upperLeftCornerY + width - 1)) {
            for (int i = upperLeftCornerX; i < upperLeftCornerX + height; ++i) {
                for (int j = upperLeftCornerY; j < upperLeftCornerY + width; ++j) {
                    weights[i][j] = -1;
                }
            }
        }
        obstacle_count -= current_area;
    }
}

template <typename T>
void make_islands(long long obstacle_count, vector<vector<T>>& weights, const Cell& start, const Cell& finish) {
    while (obstacle_count > 0) {
        long long snake_length = max(obstacle_count * 0.01, 1.);

        int snake_start_x, snake_start_y;
        do {
            snake_start_x = rand() % weights.size();
            snake_start_y = rand() % weights.back().size();
        } while (snake_start_x == start.x && snake_start_y == start.y);

        auto cur_x = snake_start_x, cur_y = snake_start_y;
        auto prev_x = cur_x, prev_y = cur_y;
        auto length_left = snake_length;

        while (length_left > 0) {
            weights[cur_x][cur_y] = -1;
            --length_left;
            set<string> available_directions = {"up", "down", "left", "right"};
            adjust_directions_choice(available_directions, cur_x, cur_y,
                                     prev_x, prev_y, weights);

            int delta = rand() % available_directions.size();
            auto it = available_directions.begin();
            while (delta > 0) {
                ++it;
                --delta;
            }
            string new_direction = *it;

            prev_x = cur_x, prev_y = cur_y;
            if (new_direction == "up") {
                cur_x = prev_x - 1;
            } else if (new_direction == "down") {
                cur_x = prev_x + 1;
            } else if (new_direction == "left") {
                cur_y = prev_y - 1;
            } else if (new_direction == "right") {
                cur_y = prev_y + 1;
            }
        }
        obstacle_count -= snake_length;
    }

    weights[start.x][start.y] = max(1, weights[start.x][start.y]);
    weights[finish.x][finish.y] = max(1, weights[finish.x][finish.y]);
}

template <typename T>
class Generator {
    string generator_type;
    double congestion_rate;

public:
    Generator() {
        generator_type = "snakes";
        congestion_rate = 0.1;
    }

    Generator(string field_type, double congestion_rate) :
              congestion_rate(congestion_rate), generator_type(field_type) {}

    void make_field(const Cell& start, const Cell& finish, vector<vector<T>>& weights, T max_weight) {
        for (size_t i = 0; i < weights.size(); ++i) {
            for (size_t j = 0; j < weights[i].size(); ++j) {
                weights[i][j] = rand() % max_weight + 1;
            }
        }

        long long obstacle_count = weights.size() * weights.back().size() * congestion_rate;
        if (generator_type == "snakes") {
            make_snakes<T>(obstacle_count, weights, start, finish);
        } else if (generator_type == "rooms") {
            make_rooms<T>(obstacle_count, weights, start, finish);
        } else if (generator_type == "rectangles") {
            make_rectangles<T>(obstacle_count, weights, start, finish);
        } else if (generator_type == "islands") {
            make_islands<T>(obstacle_count, weights, start, finish);
        }
    }
};


#endif //DRAFT_2_GENERATOR_H
