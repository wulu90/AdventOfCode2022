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
    std::strong_ordering operator<=>(const cube&) const = default;
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

    // for (auto& s : sides) {
    //     cout << s.s1.first << ' ' << s.s1.second << ' ' << s.s2.first << ' ' << s.s2.second << ' ' << s.s3.first << ' ' << s.s3.second
    //          << endl;
    // }

    cout << sides.size() << endl;
}

side get_opposite_side(const side& s) {
    side tmp = s;
    if (s.s1.first == s.s1.second) {
        tmp.s1 = {s.s1.first + 1, s.s1.first + 1};
    }
    if (s.s2.first == s.s2.second) {
        tmp.s2 = {s.s2.first + 1, s.s2.first + 1};
    }

    if (s.s3.first == s.s3.second) {
        tmp.s3 = {s.s3.first + 1, s.s3.first + 1};
    }

    return tmp;
}

bool find_other5_side(const side& s, const set<side>& sides) {
    side tmp, tmp1, tmp2, tmp3, tmp4;
    tmp1 = tmp2 = tmp3 = tmp4 = s;
    tmp                       = get_opposite_side(s);
    if (s.s1.first == s.s1.second) {
        tmp1.s1.second += 1;
        tmp1.s2.second -= 1;

        tmp2 = get_opposite_side(tmp1);

        tmp3.s1.second += 1;
        tmp3.s3.second -= 1;
        tmp4 = get_opposite_side(tmp3);
    }

    if (s.s2.first == s.s2.second) {
        tmp1.s1.second -= 1;
        tmp1.s2.second += 1;
        tmp2 = get_opposite_side(tmp1);

        tmp3.s3.second -= 1;
        tmp3.s2.second += 1;
        tmp4 = get_opposite_side(tmp3);
    }

    if (s.s3.first == s.s3.second) {
        tmp1.s1.second -= 1;
        tmp1.s3.second += 1;
        tmp2 = get_opposite_side(tmp1);

        tmp3.s2.second -= 1;
        tmp2.s3.second += 1;
        tmp4 = get_opposite_side(tmp3);
    }

    if (sides.contains(tmp) && sides.contains(tmp1) && sides.contains(tmp2) && sides.contains(tmp3) && sides.contains(tmp4)) {
        return true;
    } else
        return false;
}

cube build_cube(const side& s) {
    cube c(s.s1.first, s.s2.first, s.s3.first);

    return c;
}

void build_possible_cubes(const side& s, set<cube>& cubes_possible) {
    int x = s.s1.first;
    int y = s.s2.first;
    int z = s.s3.first;
    cube c(x, y, z);
    cubes_possible.insert(c);
    if (s.s1.first == s.s1.second)
        x -= 1;
    if (s.s2.first == s.s2.second)
        y -= 1;
    if (s.s3.first == s.s3.second)
        z -= 1;
    cube c1(x, y, z);
    cubes_possible.insert(c1);
}

void part2() {
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

    set<cube> cube_possible;

    for(auto& s:sides){
        build_possible_cubes(s,cube_possible);
    }

    set<cube> cube_set_all;
    copy(cubes.begin(), cubes.end(), inserter(cube_set_all, cube_set_all.end()));

    

    vector<side> aaa;
    for (auto& s : sides) {
        if (find_other5_side(s, sides)) {
            aaa.push_back(s);
        }
    }

    

    set<cube> cube_set_d;
    for (auto& s : aaa) {
        auto c = build_cube(s);
        if (!cube_set_all.contains(c)) {
            cube_set_d.insert(c);
        }
    }

    //  for (auto& s : aaa) {
    //     cout << s.s1.first << ' ' << s.s1.second << ' ' << s.s2.first << ' ' << s.s2.second << ' ' << s.s3.first << ' ' << s.s3.second
    //          << endl;
    // }

    cout << sides.size() - cube_set_d.size() * 6 << endl;
}

int main() {
    part1();
    part2();
}