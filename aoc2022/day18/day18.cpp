#include <algorithm>
#include <fstream>
#include <iostream>
#include <set>
#include <vector>

using namespace std;

void part1_op() {
    ifstream input("input");
    set<tuple<int, int, int>> cubes;
    int x, y, z;
    while (input.good()) {
        input >> x;
        input.ignore(1);
        input >> y;
        input.ignore(1);
        input >> z;
        input.ignore(1);
        cubes.insert({x, y, z});
    }

    auto find_neighbors = [](const tuple<int, int, int>& it) -> vector<tuple<int, int, int>> {
        vector<tuple<int, int, int>> res;
        res.push_back({get<0>(it) + 1, get<1>(it), get<2>(it)});
        res.push_back({get<0>(it) - 1, get<1>(it), get<2>(it)});
        res.push_back({get<0>(it), get<1>(it) + 1, get<2>(it)});
        res.push_back({get<0>(it), get<1>(it) - 1, get<2>(it)});
        res.push_back({get<0>(it), get<1>(it), get<2>(it) + 1});
        res.push_back({get<0>(it), get<1>(it), get<2>(it) - 1});
        return res;
    };

    int allsides = 0;
    for (auto& c : cubes) {
        int six_sides = 6;
        for (auto& nei : find_neighbors(c)) {
            if (cubes.contains(nei))
                six_sides -= 1;
        }
        allsides += six_sides;
    }
    cout << allsides << endl;
}

int main() {
    part1_op();
    // part2();
}