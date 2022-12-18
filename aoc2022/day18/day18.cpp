#include <fstream>
#include <iostream>
#include <set>
#include <vector>

using namespace std;

struct side {
    pair<int, int> s1;
    pair<int, int> s2;
    pair<int, int> s3;

    std::strong_ordering operator<=>(const side&) const = default;
};

struct cube {
    int x;
    int y;
    int z;

    vector<side> sides;
    cube() = default;

    cube(int _x, int _y, int _z) : x(_x), y(_y), z(_z) {
        sides.push_back({{x, x}, {y, y + 1}, {z, z + 1}});
        sides.push_back({{x + 1, x + 1}, {y, y + 1}, {z, z + 1}});
        sides.push_back({{x, x + 1}, {y, y}, {z, z + 1}});
        sides.push_back({{x, x + 1}, {y + 1, y + 1}, {z, z + 1}});
        sides.push_back({{x, x + 1}, {y, y + 1}, {z, z}});
        sides.push_back({{x, x + 1}, {y, y + 1}, {z + 1, z + 1}});
    }
};

void part1() {
    ifstream input("input");
    vector<cube> cubes;
    int x, y, z;
    while (input.good()) {
        cube c;
        input >> x;
        input.ignore(1);
        input >> y;
        input.ignore(1);
        input >> z;
        input.ignore(1);
        cubes.emplace_back(x, y, z);
    }
    cubes.pop_back();

    set<side> sides;
    for (auto& s : cubes[0].sides) {
        sides.insert(s);
    }

    for (size_t i = 1; i < cubes.size(); i++) {
        for (auto& s : cubes[i].sides) {
            if (sides.contains(s)) {
                sides.erase(s);
            } else {
                sides.insert(s);
            }
        }
    }

    cout << sides.size() << endl;
}

int main() {
    part1();
}