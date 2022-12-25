#include <algorithm>
#include <array>
#include <fstream>
#include <iostream>
#include <queue>
#include <string>
#include <vector>

using namespace std;

// enum class blizzard { up, right, down, left };

struct state {
    int r;
    int c;
    int minutes;
    vector<vector<vector<char>>> blimap;

    state(int _r, int _c, int minu, vector<vector<vector<char>>>& blis) : r(_r), c(_c), minutes(minu), blimap(blis) {}
};

void move_blis(vector<vector<vector<char>>>& blis, vector<vector<vector<char>>>& emptyblis) {
    auto rownum = blis.size();
    auto colnum = blis[0].size();

    auto temp = emptyblis;

    for (size_t r = 0; r < rownum; r++) {
        for (size_t c = 0; c < colnum; c++) {
            for (auto& b : blis[r][c]) {
                switch (b) {
                case 3:
                    if (r == 0)
                        temp[rownum - 1][c].push_back(3);
                    else
                        temp[r - 1][c].push_back(3);
                    break;
                case 0:
                    if (c == colnum - 1)
                        temp[r][0].push_back(0);
                    else
                        temp[r][c + 1].push_back(0);
                    break;
                case 1:
                    if (r == rownum - 1)
                        temp[0][c].push_back(1);
                    else
                        temp[r + 1][c].push_back(1);
                    break;
                case 2:
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

    state s(0, 0, count, blis);

    queue<state> qs;
    qs.push(s);

    int rownum                   = blis.size();
    int colnum                   = blis[0].size();
    array<array<int, 2>, 4> dirs = {{{-1, 0}, {0, 1}, {1, 0}, {0, -1}}};
    while (!(qs.front().r == rownum - 1 && qs.front().c == colnum - 1)) {
        cout << qs.size() << endl;
        auto& tmp = qs.front().blimap;
        auto r    = qs.front().r;
        auto c    = qs.front().c;
        move_blis(tmp, emptyblis);

        bool needwait = true;
        for (auto [a, b] : dirs) {
            if (r + a < 0 || r + a >= rownum || c + b < 0 || c + b >= colnum)
                continue;
            if (tmp[r + a][c + b].empty()) {
                state s(r + a, c + b, qs.front().minutes + 1, tmp);
                qs.push(s);
                needwait = false;
            }
        }
        if (needwait) {
            state s(r, c, qs.front().minutes + 1, tmp);
            qs.push(s);
        }
        qs.pop();
    }

    cout << qs.front().minutes + 1 << endl;
}

int main() {
    part1();
}