#include <array>
#include <chrono>
#include <fstream>
#include <iostream>
#include <set>
#include <string>
#include <vector>

using namespace std;

void move_blis(vector<vector<vector<char>>>& blis) {
    auto rownum = blis.size();
    auto colnum = blis[0].size();
    vector<vector<char>> emptyrow(colnum);
    vector<vector<vector<char>>> emptyblis(rownum, emptyrow);

    for (size_t r = 0; r < rownum; r++) {
        for (size_t c = 0; c < colnum; c++) {
            for (auto& b : blis[r][c]) {
                switch (b) {
                case 3:    // up ^
                    if (r == 0)
                        emptyblis[rownum - 1][c].push_back(3);
                    else
                        emptyblis[r - 1][c].push_back(3);
                    break;
                case 0:    // right >
                    if (c == colnum - 1)
                        emptyblis[r][0].push_back(0);
                    else
                        emptyblis[r][c + 1].push_back(0);
                    break;
                case 1:    // down v
                    if (r == rownum - 1)
                        emptyblis[0][c].push_back(1);
                    else
                        emptyblis[r + 1][c].push_back(1);
                    break;
                case 2:    // left <
                    if (c == 0)
                        emptyblis[r][colnum - 1].push_back(2);
                    else
                        emptyblis[r][c - 1].push_back(2);
                    break;
                }
            }
        }
    }
    blis = emptyblis;
}

vector<vector<vector<char>>> parse_input() {
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
    return blis;
}

int wait_start_empty(vector<vector<vector<char>>>& blis, int r, int c) {
    int count = 0;
    while (!blis[r][c].empty()) {
        move_blis(blis);
        count += 1;
    }
    return count;
}

int step_target(vector<vector<vector<char>>>& blis, int sr, int sc, int er, int ec) {
    int count = 0;

    int rownum                   = blis.size();
    int colnum                   = blis[0].size();
    array<array<int, 2>, 4> dirs = {{{-1, 0}, {0, 1}, {1, 0}, {0, -1}}};

    set<pair<int, int>> positions;
    positions.insert({sr, sc});

    bool success = true;

    while (!positions.contains({er, ec})) {
        move_blis(blis);
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

        if (tmp_p.empty()) {
            success = false;
            break;
        }

        positions = tmp_p;
        count += 1;
    }

    move_blis(blis);
    count += 1;

    if (!success)
        count = -1;

    return count;
}

int go_through(vector<vector<vector<char>>>& blis, int sr, int sc, int er, int ec) {
    auto temp = blis;

    int count = 0;
    while (true) {
        int a = wait_start_empty(blis, sr, sc);
        temp  = blis;
        count += a;
        int b = step_target(temp, sr, sc, er, ec);
        if (b == -1) {
            move_blis(blis);
            count += 1;
        } else {
            count += b;
            break;
        }
    }
    blis = temp;
    return count;
}

void part1() {
    auto blis = parse_input();
    cout << go_through(blis, 0, 0, blis.size() - 1, blis[0].size() - 1) << endl;
}

void part2() {
    auto blis = parse_input();

    auto a = go_through(blis, 0, 0, blis.size() - 1, blis[0].size() - 1);
    auto b = go_through(blis, blis.size() - 1, blis[0].size() - 1, 0, 0);
    auto c = go_through(blis, 0, 0, blis.size() - 1, blis[0].size() - 1);

    cout << a + b + c << endl;
}

int main() {
    part1();
    part2();
}