#include <iostream>
#include <map>
#include "Input_Type.h"
#include "Algo.h"
#include "Result_Type.h"
#include "Printer.h"

using namespace std;

struct Efficiency_Data {
    string field_type;
    bool path_found;
    bool uniform_weights;
    pair<int, int> field_size;
    pair<double, double> time;
    pair<long long, long long> expanded_cells_count;
    Efficiency_Data() = default;
};

ostream& operator<<(ostream& out, const Efficiency_Data& data) {
    out << data.field_type << "\t";
    out << data.uniform_weights << "\t";
    out << data.path_found << "\t";
    out << data.field_size.first << "\t" << data.field_size.second << "\t";
    out << data.time.first << "\t" << data.time.second << "\t";
    out << data.expanded_cells_count.first << "\t" << data.expanded_cells_count.second << endl;
}

void make_user_input(const bool is_uniform, const string& field_type, const pair<int, int> size_range) {
    ofstream test_out("input.txt");
    int n, m;
    n = size_range.first + rand() % size_range.second;
    m = size_range.first + rand() % size_range.second;
    test_out << n << " " << m << "\n";

    int start_x, start_y, finish_x, finish_y;
    start_x = rand() % n, start_y = rand() % m;
    finish_x = rand() % n, finish_y = rand() % m;
    test_out << start_x << " " << start_y << "\n";
    test_out << finish_x << " " << finish_y << "\n";

    test_out << "no\n"; // is manual input

    int max_weight;
    if (is_uniform) {
        max_weight = 1;
    } else {
        max_weight = 2 + rand() % 1000;
    }
    test_out << max_weight << "\n";

    test_out << field_type << "\n";

    int max_congestion_rate_percent;
    if (field_type == "snakes") {
        max_congestion_rate_percent = 20;
    } else if (field_type == "rooms") {
        max_congestion_rate_percent = 30;
    } else {
        max_congestion_rate_percent = 60;
    }
    double congestion_rate = (rand() % max_congestion_rate_percent) / 100.;
    test_out << congestion_rate << "\n";

    string heuristic_key = "mnh";
    test_out << heuristic_key << "\n";

    string optimization = "-";
    test_out << optimization << endl;
}

void gather_data(const Input_Type<int>& input,
                 const Result_Type<int>& res_A_star, const Result_Type<int>& res_Dijkstra,
                 vector<Efficiency_Data>& gathered_data) {
    Efficiency_Data data;
    data.field_type = input.get_field_type();
    data.field_size = input.get_field_size();
    data.path_found = (res_A_star.total_cost > -1);
    data.uniform_weights = input.get_max_weight() < 2;
    data.time = {res_A_star.exec_time, res_Dijkstra.exec_time};
    data.expanded_cells_count = {res_A_star.expanded_cells_count, res_Dijkstra.expanded_cells_count};
    gathered_data.push_back(data);
}

int main() {
    ofstream stat_out("statistics.txt");
    vector<Efficiency_Data> gathered_data;

    vector<string> field_types = {/*"rooms", "snakes",*/ "rectangles", "islands"};
    vector<pair<int, int>> size_ranges = {/*{1, 10}, */{500, 1000}};
    const int tests_count = 1000;

    for (int cur_field_type_index = 0; cur_field_type_index < field_types.size(); ++cur_field_type_index) {
        for (int cur_range_index = 0; cur_range_index < size_ranges.size(); ++cur_range_index) {
            for (int is_uniform = 0; is_uniform <= 0; ++is_uniform) {
                for (int test_id = 0; test_id < tests_count; ++test_id) {
                    auto size_range = size_ranges[cur_range_index];
                    cout << test_id << endl;
                    auto field_type = field_types[cur_field_type_index];
                    cout << field_type << endl;
                    make_user_input((bool)is_uniform, field_type, size_range);

                    Input_Type<int> input;
                    input.read_input();
                    Result_Type<int> res_A_star = Algo<int>::A_star(input);
                    Printer<int>::print_main_info(input, res_A_star);
                    //Printer<int>::print_map(input, res_A_star);
                    input.change_h_key("0");
                    Result_Type<int> res_Dijkstra = Algo<int>::A_star(input);
                    Printer<int>::print_main_info(input, res_Dijkstra);

                    gather_data(input, res_A_star, res_Dijkstra, gathered_data);
                }
            }
        }
    }

    for (const auto& elem : gathered_data) {
        stat_out << elem;
    }
    //close file?
    return 0;
}
