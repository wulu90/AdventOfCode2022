#include <array>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

void forward(const string& line, size_t& curr, size_t step) {
    auto wall = line.find('#', curr + 1);
    if (wall != string::npos) {
        if (wall - curr - 1 >= step) {
            curr += step;
        } else {
            curr = wall - 1;
        }
    } else {
        if (line.size() - curr - 1 >= step) {
            curr += step;
        } else {
            if (line[0] != '#') {
                step = step - (line.size() - curr - 1) - 1;
                curr = 0;
                forward(line, curr, step);
            } else {
                curr = line.size() - 1;
            }
        }
    }
}

void part1() {
    ifstream input("input");
    vector<string> force_field;
    vector<int> steps;
    vector<char> dirs;
    string line;
    getline(input, line);
    while (!line.empty()) {
        force_field.push_back(line);
        getline(input, line);
    }

    getline(input, line);    // last line, password

    size_t row    = 0;
    size_t column = 0;
    int facing    = 0;

    column = force_field[0].find_first_not_of(' ');

    int step;
    char dir;

    istringstream iss(line);

    while (iss.good()) {
        iss >> step;

        if (facing == 0) {
            auto s      = force_field[row].find_first_not_of(' ');
            string line = force_field[row].substr(s, force_field[row].size() - s);
            auto curr   = column - s;
            forward(line, curr, step);
            column = s + curr;

        } else if (facing == 1) {
            size_t s = 0;
            while (force_field[s][column] == ' ')
                s += 1;

            string line;
            for (size_t i = s; i < force_field.size(); i++) {
                if (column < force_field[i].size()) {
                    line.push_back(force_field[i][column]);
                } else
                    break;
            }

            auto curr = row - s;
            forward(line, curr, step);
            row = s + curr;
        }

        else if (facing == 2) {
            auto s      = force_field[row].find_first_not_of(' ');
            string line = force_field[row].substr(s, force_field[row].size() - s);
            string line_r(line.rbegin(), line.rend());
            auto curr = line_r.size() - (column - s) - 1;
            forward(line_r, curr, step);
            column = s + (line_r.size() - curr - 1);

        } else {
            size_t s = 0;
            while (force_field[s][column] == ' ')
                s += 1;

            string line;
            for (size_t i = s; i < force_field.size(); i++) {
                if (column < force_field[i].size()) {
                    line.push_back(force_field[i][column]);
                } else
                    break;
            }
            string line_r(line.rbegin(), line.rend());
            auto curr = line_r.size() - (row - s) - 1;
            forward(line_r, curr, step);
            row = s + (line_r.size() - curr - 1);
        }

        if (!iss.eof()) {
            iss >> dir;

            if (dir == 'R') {
                if (facing == 3)
                    facing = 0;
                else
                    facing += 1;
            } else {
                if (facing == 0)
                    facing = 3;
                else
                    facing -= 1;
            }
        }
    }

    cout << (row + 1) * 1000 + 4 * (column + 1) + facing << endl;
}

enum class direction { right, down, left, up };

void proceed(size_t& side, direction& dir, size_t& r, size_t& c) {
    /**
     *    1 0
     *    2
     *  4 3
     *  5
     *
     */

    if (side == 0) {
        if (dir == direction::right) {
            side = 3;
            dir  = direction::left;
            r    = 49 - r;
            c    = 49;
        } else if (dir == direction::down) {
            side = 2;
            dir  = direction::left;
            r    = c;
            c    = 49;
        } else if (dir == direction::left) {
            side = 1;
            dir  = direction::left;
            r    = r;
            c    = 49;
        } else {
            side = 5;
            dir  = direction::up;
            r    = 49;
            c    = c;
        }
    } else if (side == 1) {
        if (dir == direction::right) {
            side = 0;
            dir  = direction::right;
            r    = r;
            c    = 0;
        } else if (dir == direction::down) {
            side = 2;
            dir  = direction::down;
            r    = 0;
            c    = c;
        } else if (dir == direction::left) {
            side = 4;
            dir  = direction::right;
            r    = 49 - r;
            c    = 0;
        } else {
            side = 5;
            dir  = direction::right;
            r    = c;
            c    = 0;
        }
    } else if (side == 2) {
        if (dir == direction::right) {
            side = 0;
            dir  = direction::up;
            c    = r;
            r    = 49;
        } else if (dir == direction::down) {
            side = 3;
            dir  = direction::down;
            r    = 0;
            c    = c;
        } else if (dir == direction::left) {
            side = 4;
            dir  = direction::down;
            c    = r;
            r    = 0;
        } else {
            side = 1;
            dir  = direction::up;
            r    = 49;
            c    = c;
        }
    } else if (side == 3) {
        if (dir == direction::right) {
            side = 0;
            dir  = direction::left;
            r    = 49 - r;
            c    = 49;
        } else if (dir == direction::down) {
            side = 5;
            dir  = direction::left;
            r    = c;
            c    = 49;
        } else if (dir == direction::left) {
            side = 4;
            dir  = direction::left;
            r    = r;
            c    = 49;
        } else {
            side = 2;
            dir  = direction::up;
            r    = 49;
            c    = c;
        }
    } else if (side == 4) {
        if (dir == direction::right) {
            side = 3;
            dir  = direction::right;
            r    = r;
            c    = 0;
        } else if (dir == direction::down) {
            side = 5;
            dir  = direction::down;
            r    = 0;
            c    = c;
        } else if (dir == direction::left) {
            side = 1;
            dir  = direction::right;
            r    = 49 - r;
            c    = 0;
        } else {
            side = 2;
            dir  = direction::right;
            r    = c;
            c    = 0;
        }
    } else {
        if (dir == direction::right) {
            side = 3;
            dir  = direction::up;
            c    = r;
            r    = 49;
        } else if (dir == direction::down) {
            side = 0;
            dir  = direction::down;
            r    = 0;
            c    = c;
        } else if (dir == direction::left) {
            side = 1;
            dir  = direction::down;
            c    = r;
            r    = 0;
        } else {
            side = 4;
            dir  = direction::up;
            r    = 49;
            c    = c;
        }
    }
}

void cube_forward(const array<array<string, 50>, 6>& cubes, size_t step, size_t& side, direction& dir, size_t& r, size_t& c) {
    size_t side_tmp   = side;
    direction dir_tmp = dir;
    size_t r_tmp      = r;
    size_t c_tmp      = c;
    if (dir == direction::right) {
        while (step > 0) {
            if (c == 49) {
                side_tmp = side;
                dir_tmp  = dir;
                r_tmp    = r;
                c_tmp    = c;
                proceed(side_tmp, dir_tmp, r_tmp, c_tmp);
                if (cubes[side_tmp][r_tmp][c_tmp] != '#') {
                    side = side_tmp;
                    dir  = dir_tmp;
                    r    = r_tmp;
                    c    = c_tmp;
                    cube_forward(cubes, step - 1, side, dir, r, c);
                    break;
                } else
                    break;

            } else {
                if (cubes[side][r][c + 1] != '#') {
                    c += 1;
                    step -= 1;
                } else
                    break;
            }
        }
    }

    else if (dir == direction::down) {
        while (step > 0) {
            if (r == 49) {
                side_tmp = side;
                dir_tmp  = dir;
                r_tmp    = r;
                c_tmp    = c;
                proceed(side_tmp, dir_tmp, r_tmp, c_tmp);
                if (cubes[side_tmp][r_tmp][c_tmp] != '#') {
                    side = side_tmp;
                    dir  = dir_tmp;
                    r    = r_tmp;
                    c    = c_tmp;
                    cube_forward(cubes, step - 1, side, dir, r, c);
                    break;
                } else
                    break;

            } else {
                if (cubes[side][r + 1][c] != '#') {
                    r += 1;
                    step -= 1;
                } else
                    break;
            }
        }
    } else if (dir == direction::left) {
        while (step > 0) {
            if (c == 0) {
                side_tmp = side;
                dir_tmp  = dir;
                r_tmp    = r;
                c_tmp    = c;
                proceed(side_tmp, dir_tmp, r_tmp, c_tmp);
                if (cubes[side_tmp][r_tmp][c_tmp] != '#') {
                    side = side_tmp;
                    dir  = dir_tmp;
                    r    = r_tmp;
                    c    = c_tmp;
                    cube_forward(cubes, step - 1, side, dir, r, c);
                    break;
                } else
                    break;

            } else {
                if (cubes[side][r][c - 1] != '#') {
                    c -= 1;
                    step -= 1;
                } else
                    break;
            }
        }
    } else {
        while (step > 0) {
            if (r == 0) {
                side_tmp = side;
                dir_tmp  = dir;
                r_tmp    = r;
                c_tmp    = c;
                proceed(side_tmp, dir_tmp, r_tmp, c_tmp);
                if (cubes[side_tmp][r_tmp][c_tmp] != '#') {
                    side = side_tmp;
                    dir  = dir_tmp;
                    r    = r_tmp;
                    c    = c_tmp;
                    cube_forward(cubes, step - 1, side, dir, r, c);
                    break;
                } else
                    break;
            } else {
                if (cubes[side][r - 1][c] != '#') {
                    r -= 1;
                    step -= 1;
                } else {
                    break;
                }
            }
        }
    }
}

void part2() {
    ifstream input("input");

    array<array<string, 50>, 6> cubes;

    array<string, 50> side0;
    array<string, 50> side1;
    array<string, 50> side2;
    array<string, 50> side3;
    array<string, 50> side4;
    array<string, 50> side5;

    string line;
    for (int i = 0; i < 200; i++) {
        getline(input, line);
        if (i >= 0 && i < 50) {
            side0[i] = line.substr(100, 50);
            side1[i] = line.substr(50, 50);
        }
        if (i >= 50 && i < 100) {
            side2[i - 50] = line.substr(50, 50);
        }
        if (i >= 100 && i < 150) {
            side3[i - 100] = line.substr(50, 50);
            side4[i - 100] = line.substr(0, 50);
        }
        if (i >= 150) {
            side5[i - 150] = line;
        }
    }

    cubes[0] = side0;
    cubes[1] = side1;
    cubes[2] = side2;
    cubes[3] = side3;
    cubes[4] = side4;
    cubes[5] = side5;

    getline(input, line);
    getline(input, line);

    istringstream iss(line);

    int step;
    char d;

    size_t side   = 1;
    size_t r      = 0;
    size_t c      = 0;
    direction dir = direction::right;
    while (iss.good()) {
        iss >> step;

        cube_forward(cubes, step, side, dir, r, c);

        if (!iss.eof()) {
            iss >> d;
            if (d == 'R') {
                if (dir == direction::right)
                    dir = direction::down;
                else if (dir == direction::down)
                    dir = direction::left;
                else if (dir == direction::left)
                    dir = direction::up;
                else
                    dir = direction::right;

            } else {
                if (dir == direction::right)
                    dir = direction::up;
                else if (dir == direction::down)
                    dir = direction::right;
                else if (dir == direction::left)
                    dir = direction::down;
                else
                    dir = direction::left;
            }
        }
    }

    cout << "side " << side << endl;
    cout << "r " << r << endl;
    cout << "c " << c << endl;

    int rs = 0;
    int cs = 0;
    if (side == 0) {
        rs = 0;
        cs = 100;
    } else if (side == 1) {
        rs = 0;
        cs = 50;
    } else if (side == 2) {
        rs = 50;
        cs = 50;
    } else if (side == 3) {
        rs = 100;
        cs = 50;
    } else if (side == 4) {
        rs = 100;
        cs = 0;
    } else {
        rs = 150;
        cs = 0;
    }

    int facing = 0;
    if (dir == direction::right)
        facing = 0;
    else if (dir == direction::down)
        facing = 1;
    else if (dir == direction::left)
        facing = 2;
    else if (dir == direction::up)
        facing = 3;

    cout << "row " << (r + rs + 1) << endl;
    cout << "col  " << (c + cs + 1) << endl;
    cout << "facing " << facing << endl;
    cout << (r + rs + 1) * 1000 + (c + cs + 1) * 4 + facing << endl;
}

int main() {
    part1();
    part2();
}