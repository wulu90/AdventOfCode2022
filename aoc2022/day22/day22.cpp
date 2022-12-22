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

int main() {
    part1();
}