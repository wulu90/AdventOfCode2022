#include <algorithm>
#include <fstream>
#include <iostream>
#include <set>
#include <vector>

using namespace std;

struct cube {
    int x;
    int y;
    int z;

    bool operator<(const cube& r) const {
        return tie(x, y, z) < tie(r.x, r.y, r.z);
    }

    cube(int _x, int _y, int _z) : x(_x), y(_y), z(_z) {}
};

void part1_op() {
    ifstream input("input");
    set<cube> cubes;
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

    auto find_neighbors = [](const cube& i) -> vector<cube> {
        vector<cube> res;
        res.push_back({i.x + 1, i.y, i.z});
        res.push_back({i.x - 1, i.y, i.z});
        res.push_back({i.x, i.y + 1, i.z});
        res.push_back({i.x, i.y - 1, i.z});
        res.push_back({i.x, i.y, i.z + 1});
        res.push_back({i.x, i.y, i.z - 1});

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