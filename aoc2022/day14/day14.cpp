#include <fstream>
#include <iostream>
#include <sstream>
#include <stack>
#include <string>
#include <vector>

using namespace std;

struct coord {
    int x;
    int y;
};

vector<coord> parse_rock_path(const string& line) {
    vector<coord> path;
    coord co;
    char c;
    istringstream iss(line);
    int i;
    while (!iss.eof()) {
        iss >> i;
        co.x = i;
        iss >> c;    // ,
        iss >> i;
        co.y = i;
        path.push_back(co);
        iss.ignore(4);    // " -> "
    }
    return path;
}

void part1() {
    ifstream input("input");
    string line;
    vector<vector<coord>> path_all;
    while (getline(input, line)) {
        path_all.push_back(parse_rock_path(line));
    }

    int xmax = 0;
    int ymax = 0;
    for (auto& path : path_all) {
        for (auto& c : path) {
            if (c.x > xmax)
                xmax = c.x;
            if (c.y > ymax)
                ymax = c.y;
        }
    }
    vector<bool> row(xmax + 1);
    vector<vector<bool>> rockmap(ymax + 1, row);
    auto sandmap = rockmap;

    for (auto& path : path_all) {
        for (size_t i = 0; i < path.size() - 1; i++) {
            if (path[i].x == path[i + 1].x) {
                int sy = min(path[i].y, path[i + 1].y);
                int ey = max(path[i].y, path[i + 1].y);
                for (int j = sy; j <= ey; j++) {
                    rockmap[j][path[i].x] = true;
                }
            }

            if (path[i].y == path[i + 1].y) {
                int sx = min(path[i].x, path[i + 1].x);
                int ex = max(path[i].x, path[i + 1].x);
                for (int j = sx; j <= ex; j++) {
                    rockmap[path[i].y][j] = true;
                }
            }
        }
    }

    vector<int> dir{0, 1, -1, 1, 1, 1};

    stack<coord> sand_stack;
    sand_stack.push({500, 0});
    bool endless = false;
    int count    = 0;
    while (!sand_stack.empty()) {
        auto cur = sand_stack.top();

        bool can_rest = true;
        for (int i = 0; i < 3; i++) {
            coord next{cur.x + dir[i * 2], cur.y + dir[i * 2 + 1]};

            if (next.x > xmax || next.y > ymax) {
                endless = true;
            } else {
                if (!rockmap[next.y][next.x] && !sandmap[next.y][next.x]) {
                    sand_stack.push(next);
                    can_rest = false;
                    break;
                }
            }
        }
        if (endless)
            break;
        if (can_rest) {
            count++;
            sandmap[sand_stack.top().y][sand_stack.top().x] = true;
            sand_stack.pop();
        }
    }

    cout << count << endl;
}

void part2() {
    ifstream input("input");
    string line;
    vector<vector<coord>> path_all;
    while (getline(input, line)) {
        path_all.push_back(parse_rock_path(line));
    }

    int xmax = 0;
    int ymax = 0;
    for (auto& path : path_all) {
        for (auto& c : path) {
            if (c.x > xmax)
                xmax = c.x;
            if (c.y > ymax)
                ymax = c.y;
        }
    }

    int floor = ymax + 2;

    vector<bool> row(xmax + 1 + floor);
    vector<vector<bool>> rockmap(ymax + 2, row);    // +1 for the gap between last rock row and floor
    vector<bool> floorrow(xmax + 1 + floor, true);
    rockmap.push_back(floorrow);
    auto sandmap = rockmap;

    for (auto& path : path_all) {
        for (size_t i = 0; i < path.size() - 1; i++) {
            if (path[i].x == path[i + 1].x) {
                int sy = min(path[i].y, path[i + 1].y);
                int ey = max(path[i].y, path[i + 1].y);
                for (int j = sy; j <= ey; j++) {
                    rockmap[j][path[i].x] = true;
                }
            }

            if (path[i].y == path[i + 1].y) {
                int sx = min(path[i].x, path[i + 1].x);
                int ex = max(path[i].x, path[i + 1].x);
                for (int j = sx; j <= ex; j++) {
                    rockmap[path[i].y][j] = true;
                }
            }
        }
    }

    vector<int> dir{0, 1, -1, 1, 1, 1};

    stack<coord> sand_stack;
    sand_stack.push({500, 0});

    int count = 0;
    while (!sand_stack.empty()) {
        auto cur = sand_stack.top();

        bool can_rest = true;
        for (int i = 0; i < 3; i++) {
            coord next{cur.x + dir[i * 2], cur.y + dir[i * 2 + 1]};

            if (!rockmap[next.y][next.x] && !sandmap[next.y][next.x]) {
                sand_stack.push(next);
                can_rest = false;
                break;
            }
        }

        if (can_rest) {
            count++;
            sandmap[sand_stack.top().y][sand_stack.top().x] = true;
            sand_stack.pop();
        }
    }

    cout << count << endl;
}

int main() {
    part1();
    part2();
}