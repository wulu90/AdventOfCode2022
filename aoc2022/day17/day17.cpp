#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <tuple>
#include <vector>

using namespace std;

/**
 *
 *   ####
 *
 *   .#.
 *   ###
 *   .#.
 *
 *   ..#
 *   ..#
 *   ###
 *
 *   #
 *   #
 *   #
 *   #
 *
 *   ##
 *   ##
 */

void solve(int64_t total_rocks) {
    ifstream input("input");
    string wind;

    getline(input, wind);

    vector<int> tower;
    tower.push_back(0x7F);

    int wind_index     = 0;
    int64_t rock_count = 0;
    int windsize       = wind.size();

    int64_t cycle_heights = 0L;
    bool cycle_found      = false;

    map<tuple<int, int, vector<int>>, pair<int, int>> cycle;

    vector<vector<int>> rocks = {{0x1E}, {0x08, 0x1C, 0x08}, {0x1C, 0x04, 0x04}, {0x10, 0x10, 0x10, 0x10}, {0x18, 0x18}};

    vector<vector<int>> lr_side = {{-2, 1}, {-2, 2}, {-2, 2}, {-2, 4}, {-2, 3}};    // -3,-2,-1,0,1,2,3

    auto rock_not_stop = [&](int rock_inx, int shift, int rock_bottom) -> bool {
        for (size_t i = 0; i < rocks[rock_inx].size(); i++) {
            int shift_value = shift > 0 ? rocks[rock_inx][i] >> shift : rocks[rock_inx][i] << -shift;
            if (rock_bottom + i < tower.size() && (shift_value & tower[rock_bottom + i]) > 0) {
                return false;
            }
        }
        return true;
    };

    /**
     *
     *  |#####..|
     *  |..###..|
     *  |...#...|
     *  |..####.|
     *  +-------+
     *   3333310   surfcace : every colume top # to the lowest
     */
    auto get_tower_surf = [&]() -> vector<int> {
        vector<int> res;
        for (int i = 0; i < 7; i++) {
            for (auto t = tower.rbegin(); t != tower.rend(); t++) {
                if ((*t) & (1 << i)) {
                    res.push_back(distance(t, tower.rend() - 1));
                    break;
                }
            }
        }

        auto minv = min_element(res.begin(), res.end());
        transform(res.begin(), res.end(), res.begin(), [minv](auto& v) { return v - *minv; });
        return res;
    };

    while (rock_count < total_rocks) {
        int rock_bottom_p = tower.size() + 3;
        int shift         = 0;
        bool drop         = true;
        do {
            switch (wind[wind_index]) {
            case '>':
                if (shift != lr_side[rock_count % 5][1] && rock_not_stop(rock_count % 5, shift + 1, rock_bottom_p))
                    shift += 1;
                break;

            case '<':
                if (shift != lr_side[rock_count % 5][0] && rock_not_stop(rock_count % 5, shift - 1, rock_bottom_p))
                    shift -= 1;
                break;
            }

            wind_index = (wind_index + 1) % windsize;
            drop       = rock_not_stop(rock_count % 5, shift, rock_bottom_p - 1);
            if (drop)
                rock_bottom_p -= 1;
        } while (drop);

        size_t rock_top_p = rock_bottom_p + rocks[rock_count % 5].size();
        if (tower.size() < rock_top_p)
            tower.resize(rock_top_p);

        for (size_t i = 0; i < rocks[rock_count % 5].size(); i++) {
            int shift_v = shift > 0 ? rocks[rock_count % 5][i] >> shift : rocks[rock_count % 5][i] << -shift;
            tower[rock_bottom_p + i] |= shift_v;
        }

        rock_count += 1;

        if (!cycle_found) {
            // rock_index, wind_index, tower_surface
            int rock_index            = (rock_count - 1) % 5;
            auto rock_index_wind_surf = make_tuple(rock_index, (wind_index - 1 + windsize) % windsize, get_tower_surf());
            // rock_count, tower_height
            auto rock_count_height = make_pair((rock_count - 1), tower.size() - 1);

            if (cycle.contains(rock_index_wind_surf)) {
                // cout << (rock_index - 1) % 5 << "  " << (wind_index - 1 + windsize) % windsize << "  " << tower[tower.size() - 1] << "  "
                //      << tower.size() << endl;

                cycle_found     = true;
                auto cy_rocks   = rock_count - 1 - cycle[rock_index_wind_surf].first;
                auto cy_heights = tower.size() - 1 - cycle[rock_index_wind_surf].second;

                auto left_rocks     = (total_rocks - 1) - (rock_count - 1);
                auto left_cys       = left_rocks / cy_rocks;
                auto reminder_rocks = left_rocks % cy_rocks;

                // change rock_index to all cycles done;
                rock_count = total_rocks - reminder_rocks;

                cycle_heights = left_cys * cy_heights;
            }

            cycle[rock_index_wind_surf] = rock_count_height;
        }
        // if (rock_index == 2022) {
        //     cout << tower.size() - 1 << endl;
        //     return;
        // }
    }

    cout << tower.size() - 1 + cycle_heights << endl;
}

int main() {
    solve(2022);
    solve(1000000000000);
}