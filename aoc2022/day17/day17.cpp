#include <fstream>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

/**
 *
####

.#.
###
.#.

..#
..#
###

#
#
#
#

##
##
*/

void part1() {
    ifstream input("input");
    string wind;

    getline(input, wind);

    vector<int> tower;
    tower.push_back(0x7F);

    int wind_index  = 0;
    int stone_index = 0;

    vector<vector<int>> stones = {{0x1E}, {0x08, 0x1C, 0x08}, {0x1C, 0x04, 0x04}, {0x10, 0x10, 0x10, 0x10}, {0x18, 0x18}};

    vector<vector<int>> lr_side = {{-2, 1}, {-2, 2}, {-2, 2}, {-2, 4}, {-2, 3}};    // -3,-2,-1,0,1,2,3

    auto stone_not_stop = [&](int stone_inx, int shift, int stone_bottom) -> bool {
        for (size_t i = 0; i < stones[stone_inx].size(); i++) {
            int shift_value = shift > 0 ? stones[stone_inx][i] >> shift : stones[stone_inx][i] << -shift;
            if (stone_bottom + i < tower.size() && (shift_value & tower[stone_bottom + i]) > 0) {
                return false;
            }
        }
        return true;
    };

    while (true) {
        int stone_bottom_p = tower.size() + 3;
        int shift          = 0;
        bool drop          = true;
        do {
            switch (wind[wind_index]) {
            case '>':
                if (shift != lr_side[stone_index % 5][1] && stone_not_stop(stone_index % 5, shift + 1, stone_bottom_p))
                    shift += 1;
                break;

            case '<':
                if (shift != lr_side[stone_index % 5][0] && stone_not_stop(stone_index % 5, shift - 1, stone_bottom_p))
                    shift -= 1;
                break;
            }

            wind_index = (wind_index + 1) % wind.size();
            drop       = stone_not_stop(stone_index % 5, shift, stone_bottom_p - 1);
            if (drop)
                stone_bottom_p -= 1;
        } while (drop);

        size_t stone_top_p = stone_bottom_p + stones[stone_index % 5].size();
        if (tower.size() < stone_top_p)
            tower.resize(stone_top_p);

        for (size_t i = 0; i < stones[stone_index % 5].size(); i++) {
            int shift_v = shift > 0 ? stones[stone_index % 5][i] >> shift : stones[stone_index % 5][i] << -shift;
            tower[stone_bottom_p + i] |= shift_v;
        }

        stone_index += 1;

        if (stone_index == 2022) {
            cout << tower.size() - 1 << endl;
            return;
        }
    }
}

int main() {
    part1();
}