#include <algorithm>
#include <chrono>
#include <fstream>
#include <iostream>
#include <set>
#include <string>
#include <vector>

using namespace std;

struct coord {
    int x;
    int y;

    std::strong_ordering operator<=>(const coord&) const = default;
};

struct sensor_beacon {
    coord sensor;
    coord beacon;
};

int manhattan_distance(const sensor_beacon& sb) {
    return abs(sb.beacon.x - sb.sensor.x) + abs(sb.beacon.y - sb.sensor.y);
}

int manhattan_distance(const coord& a, const coord& b) {
    return abs(a.x - b.x) + abs(a.y - b.y);
}

vector<coord> impossible_pos(const sensor_beacon& sb, const int target) {
    vector<coord> ips;
    int md = manhattan_distance(sb);

    if ((sb.sensor.y <= target && sb.sensor.y + md >= target) || (sb.sensor.y >= target && sb.sensor.y - md <= target)) {
        int xr = md - abs(target - sb.sensor.y);
        for (int i = 1; i <= xr; i++) {
            ips.push_back({sb.sensor.x + i, target});
            ips.push_back({sb.sensor.x - i, target});
        }
        ips.push_back({sb.sensor.x, target});
    }

    return ips;
}

sensor_beacon parse_sb(const string& line) {
    sensor_beacon sb;

    size_t start = line.find_first_of('x');
    size_t end   = line.find_first_of(',');
    sb.sensor.x  = stoi(line.substr(start + 2, end - start - 2));
    start        = line.find_first_of('y');
    end          = line.find_first_of(':');
    sb.sensor.y  = stoi(line.substr(start + 2, end - start - 2));

    start       = line.find_last_of('x');
    end         = line.find_last_of(',');
    sb.beacon.x = stoi(line.substr(start + 2, end - start - 2));
    start       = line.find_last_of('y');
    sb.beacon.y = stoi(line.substr(start + 2, line.size() - start - 2));

    return sb;
}

void part1() {
    ifstream input("input");
    vector<sensor_beacon> sbs;
    string line;
    while (getline(input, line)) {
        sbs.push_back(parse_sb(line));
    }

    vector<coord> impossible;
    const int target = 2000000;
    set<coord> remove;
    for (auto& sb : sbs) {
        auto ips = impossible_pos(sb, target);
        impossible.insert(impossible.end(), ips.begin(), ips.end());
        if (sb.beacon.y == target) {
            remove.insert(sb.beacon);
        }
        if (sb.sensor.y == target) {
            remove.insert(sb.sensor);
        }
    }
    set<coord> ips_set;
    int dup = 0;
    // too large, build set need so much time;
    copy(impossible.begin(), impossible.end(), inserter(ips_set, ips_set.end()));
    for (auto& r : remove) {
        if (ips_set.find(r) != ips_set.end()) {
            dup++;
        }
    }

    cout << ips_set.size() - dup << endl;
}

void part1_1() {
    ifstream input("input");
    vector<sensor_beacon> sbs;
    string line;
    while (getline(input, line)) {
        sbs.push_back(parse_sb(line));
    }

    static const int target = 2000000;
    set<coord> dup;
    for (auto& sb : sbs) {
        if (sb.beacon.y == target)
            dup.insert(sb.beacon);
    }

    int xmin = 0xffffffff;
    int xmax = 0;

    for (auto& sb : sbs) {
        int md = manhattan_distance(sb);

        if ((sb.sensor.y <= target && sb.sensor.y + md >= target) || (sb.sensor.y >= target && sb.sensor.y - md <= target)) {
            int xr = md - abs(target - sb.sensor.y);
            if (sb.sensor.x + xr > xmax)
                xmax = sb.sensor.x + xr;
            if (sb.sensor.x - xr < xmin)
                xmin = sb.sensor.x - xr;
        }
    }

    vector<int> mds;
    for (auto& sb : sbs) {
        mds.push_back(manhattan_distance(sb));
    }

    int count = 0;
    for (int x = xmin; x <= xmax; x++) {
        // bool f = false;
        coord c{x, target};
        if (dup.find(c) != dup.end())
            continue;
        for (size_t i = 0; i < sbs.size(); i++) {
            if (manhattan_distance(c, sbs[i].sensor) <= mds[i]) {
                count++;
                break;
            }
        }
    }

    cout << count << endl;
}

void part2() {
    ifstream input("input");
    vector<sensor_beacon> sbs;
    string line;
    while (getline(input, line)) {
        sbs.push_back(parse_sb(line));
    }
    bool found     = false;
    long frequency = 0;
    vector<int> mds;
    for (auto& sb : sbs) {
        mds.push_back(manhattan_distance(sb));
    }
    for (int i = 0; i < 4000000; i++) {
        for (int j = 0; j < 4000000; j++) {
            unsigned long flag = 1UL;

            for (size_t k = 0; k < sbs.size(); k++) {
                if (abs(sbs[k].sensor.x - i) + abs(sbs[k].sensor.y - j) > mds[k]) {
                    flag <<= 1;
                } else {
                    break;
                }
            }

            if (flag == 1UL << sbs.size()) {
                found     = true;
                frequency = static_cast<long>(i) * 4000000 + j;
                break;
            }
        }
        if (found) {
            break;
        }
    }
    cout << frequency << endl;
}

bool is_overlap(int a, int b, int c, int d) {
    return !(b < c || a > d);
}

pair<int, int> extend(int a, int b, int c, int d) {
    return {min(a, c), max(b, d)};
}

bool compute(vector<int>& v) {
    auto s1 = v[0];
    auto s2 = v[1];

    vector<int> tmp;
    bool found = false;
    while (v.size() > 1) {
        tmp.clear();
        int range_count = v.size() / 2;

        for (int i = 0; i < range_count; i++) {
            if (is_overlap(s1, s2, v[i * 2], v[i * 2 + 1])) {
                auto ext = extend(s1, s2, v[i * 2], v[i * 2 + 1]);
                s1       = ext.first;
                s2       = ext.second;
            } else {
                tmp.push_back(v[i * 2]);
                tmp.push_back(v[i * 2 + 1]);
            }
        }
        if (v.size() != tmp.size()) {
            swap(v, tmp);
        } else {    // can't deduce size mean there is a gap beteen two ranges ,so found it
            found = true;
            break;
        }
    }
    if (found) {
        v.clear();
        v.insert(v.end(), {s1, s2});
        return true;
    } else {
        return false;
    }
}

void part2_op() {
    ifstream input("input");
    vector<sensor_beacon> sbs;
    string line;
    while (getline(input, line)) {
        sbs.push_back(parse_sb(line));
    }

    vector<vector<int>> overlaps(4000000 + 1);

    for (auto& sb : sbs) {
        int md = manhattan_distance(sb);

        int start_y = sb.sensor.y - md > 0 ? sb.sensor.y - md : 0;
        int end_y   = sb.sensor.y + md > 4000000 ? 4000000 : sb.sensor.y + md;

        for (int i = start_y; i <= end_y; i++) {
            int d       = md - abs(i - sb.sensor.y);
            int start_x = sb.sensor.x - d > 0 ? sb.sensor.x - d : 0;
            int end_x   = sb.sensor.x + d > 4000000 ? 4000000 : sb.sensor.x + d;

            overlaps[i].push_back(start_x);
            overlaps[i].push_back(end_x);
        }
    }

    long y = 0;

    for (int i = 0; i < 4000000; i++) {
        if (compute(overlaps[i])) {
            y = i;
            break;
        }
    }
    long x = 0;
    if (overlaps[y][0] == 0) {
        x = overlaps[y][1] + 1;
    }
    if (overlaps[y][1] == 4000000) {
        x = overlaps[y][0] - 1;
    }
    cout << x * 4000000 + y << endl;
}

int main() {
    part1_1();

    // part1();

    part2_op();
}