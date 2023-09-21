#include <algorithm>
#include <fstream>
#include <iostream>
#include <queue>
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

    // if a cube has a neighbor in one direction (a cube have six dirction), they have same panel
    // it remove by one ,it's neighbor remove by one

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

void part2() {
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

    int lo = min({cubes.begin()->x, cubes.begin()->y, cubes.begin()->z}) - 1;
    int up = max({cubes.rbegin()->x, cubes.rbegin()->y, cubes.rbegin()->z}) + 1;

    auto in_boundary = [lo, up](const cube& c) -> bool {
        return c.x >= lo && c.y >= lo && c.z >= lo && c.x <= up && c.y <= up && c.z <= up;
    };

    // flood fill algotithm
    // find all cubes out of the input cubes in the boundary
    // a i_cube has neighbor in o_cubes, it have a direction toward to outside

    cube sc(lo, lo, lo);    // flood fill start

    queue<cube> q;
    q.push(sc);

    set<cube> colored_cubes;

    while (!q.empty()) {
        int qsize = q.size();
        for (int i = 0; i < qsize; i++) {
            auto& c = q.front();

            if (!colored_cubes.contains(c)) {
                for (auto& n : find_neighbors(c)) {
                    if (in_boundary(n) && !cubes.contains(n) && !colored_cubes.contains(n)) {
                        q.push(n);
                    }
                }
                colored_cubes.insert(c);
            }
            q.pop();
        }
    }

    int outf = 0;
    for (auto& c : cubes) {
        int f = 0;
        for (auto& n : find_neighbors(c)) {
            if (colored_cubes.contains(n)) {
                f += 1;
            }
        }

        outf += f;
    }

    cout << outf << endl;
}

int main() {
    part1_op();
    part2();
}
