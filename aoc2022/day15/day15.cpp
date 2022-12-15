#include <algorithm>
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
    copy_if(impossible.begin(), impossible.end(), inserter(ips_set, ips_set.end()),
            [&](auto& c) { return remove.find(c) == remove.end(); });

    cout << ips_set.size() << endl;
}

int main() {
    part1();
}