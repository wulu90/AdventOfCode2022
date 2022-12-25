#include <array>
#include <chrono>
#include <fstream>
#include <iostream>
#include <set>
#include <string>
#include <vector>

using namespace std;

void move_blis(vector<vector<vector<char>>>& blis, vector<vector<vector<char>>>& emptyblis) {
    auto rownum = blis.size();
    auto colnum = blis[0].size();

    auto temp = emptyblis;

    for (size_t r = 0; r < rownum; r++) {
        for (size_t c = 0; c < colnum; c++) {
            for (auto& b : blis[r][c]) {
                switch (b) {
                case 3:    // up ^
                    if (r == 0)
                        temp[rownum - 1][c].push_back(3);
                    else
                        temp[r - 1][c].push_back(3);
                    break;
                case 0:    // right >
                    if (c == colnum - 1)
                        temp[r][0].push_back(0);
                    else
                        temp[r][c + 1].push_back(0);
                    break;
                case 1:    // down v
                    if (r == rownum - 1)
                        temp[0][c].push_back(1);
                    else
                        temp[r + 1][c].push_back(1);
                    break;
                case 2:    // left <
                    if (c == 0)
                        temp[r][colnum - 1].push_back(2);
                    else
                        temp[r][c - 1].push_back(2);
                    break;
                }
            }
        }
    }
    blis = temp;
}

void part1() {
    ifstream input("input");
    string line;

    vector<char> cell;
    vector<vector<char>> row;
    vector<vector<vector<char>>> blis;

    while (getline(input, line)) {
        if (line.find_first_of("<>^v") != string::npos) {
            row.clear();
            for (auto it = line.begin() + 1; it < line.end() - 1; it++) {
                cell.clear();
                if (*it == '>')
                    cell.push_back(0);
                if (*it == 'v')
                    cell.push_back(1);
                if (*it == '<')
                    cell.push_back(2);
                if (*it == '^')
                    cell.push_back(3);
                row.push_back(cell);
            }
            blis.push_back(row);
        }
    }

    row.clear();
    row.resize(blis[0].size());
    vector<vector<vector<char>>> emptyblis(blis.size(), row);

    int count = 0;
    while (!blis[0][0].empty()) {
        move_blis(blis, emptyblis);
        count += 1;
    }

    int rownum                   = blis.size();
    int colnum                   = blis[0].size();
    array<array<int, 2>, 4> dirs = {{{-1, 0}, {0, 1}, {1, 0}, {0, -1}}};

    set<pair<int, int>> positions;
    positions.insert({0, 0});

    while (!positions.contains({rownum - 1, colnum - 1})) {
        move_blis(blis, emptyblis);
        set<pair<int, int>> tmp_p;

        for (auto& [r, c] : positions) {
            for (auto& [a, b] : dirs) {
                if (r + a < 0 || r + a >= rownum || c + b < 0 || c + b >= colnum)
                    continue;
                if (blis[r + a][c + b].empty()) {
                    tmp_p.insert({r + a, c + b});
                }
            }

            if (blis[r][c].empty())    // wait
                tmp_p.insert({r, c});
        }

        positions = tmp_p;
        count += 1;
    }

    cout << count + 1 << endl;
}

int main() {
    part1();
}