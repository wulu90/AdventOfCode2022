#include <array>
#include <fstream>
#include <iostream>
#include <queue>
#include <string>
#include <vector>

using namespace std;

pair<int, int> move_to(const vector<string>& grove, const vector<array<pair<int, int>, 3>>& dirs, int r, int c) {
    if (grove[r - 1][c - 1] == '.' && grove[r - 1][c] == '.' && grove[r - 1][c + 1] == '.' && grove[r][c - 1] == '.' &&
        grove[r][c + 1] == '.' && grove[r + 1][c - 1] == '.' && grove[r + 1][c] == '.' && grove[r + 1][c + 1] == '.') {
        return {r, c};
    }

    for (int k = 0; k < 4; k++) {
        auto dir   = dirs[k];
        int rownum = grove.size();

        if (r + dir[0].first == 0 || r + dir[0].first == rownum - 1 || c + dir[0].second == 0 || c + dir[0].second == rownum - 1) {
            return {r + dir[0].first, c + dir[0].second};
            // break;
        }

        if (grove[r + dir[0].first][c + dir[0].second] == '.' && grove[r + dir[1].first][c + dir[1].second] == '.' &&
            grove[r + dir[2].first][c + dir[2].second] == '.') {
            return {r + dir[0].first, c + dir[0].second};
        }
    }
    return {r, c};
}

void part1() {
    ifstream input("input");
    vector<string> grove;
    string line;
    while (getline(input, line)) {
        grove.push_back(line);
    }

    vector<array<pair<int, int>, 3>> dirs;

    dirs.push_back({{{-1, 0}, {-1, -1}, {-1, 1}}});
    dirs.push_back({{{1, 0}, {1, -1}, {1, 1}}});
    dirs.push_back({{{0, -1}, {-1, -1}, {1, -1}}});
    dirs.push_back({{{0, 1}, {-1, 1}, {1, 1}}});

    for (int i = 0; i < 10; i++) {
        string emptyline(grove[0].size() + 2, '.');
        vector<string> newgrove;
        newgrove.push_back(emptyline);
        for (auto& s : grove) {
            string tmp(".");
            tmp.append(s);
            tmp.push_back('.');
            newgrove.push_back(tmp);
        }
        newgrove.push_back(emptyline);

        vector<pair<size_t, size_t>> from;
        vector<vector<pair<size_t, size_t>>> r(newgrove[0].size() + 2, from);
        vector<vector<vector<pair<size_t, size_t>>>> considers(newgrove.size() + 2, r);

        for (size_t r = 0; r < newgrove.size(); r++) {
            for (size_t c = 0; c < newgrove[0].size(); c++) {
                if (newgrove[r][c] == '#') {
                    auto [a, b] = move_to(newgrove, dirs, r, c);
                    considers[a][b].push_back({r, c});
                }
            }
        }

        vector<string> emptygrove(newgrove.size(), emptyline);

        for (size_t r = 0; r < newgrove.size(); r++) {
            for (size_t c = 0; c < newgrove[0].size(); c++) {
                if (considers[r][c].size() == 0) {
                    continue;
                } else if (considers[r][c].size() == 1) {
                    emptygrove[r][c] = '#';
                } else {
                    for (auto [a, b] : considers[r][c]) {
                        emptygrove[a][b] = '#';
                    }
                }
            }
        }

        grove = emptygrove;
        dirs.push_back(dirs[0]);
        dirs.erase(dirs.begin());
        // cout << i << endl;
        // for (auto& s : grove) {
        //     for (auto& c : s) {
        //         cout << c;
        //     }
        //     cout << endl;
        // }
    }
    int rownum = grove.size();
    int colnum = grove[0].size();

    int r1 = 0;
    for (int i = 0; i < rownum; i++) {
        if (grove[i].find('#') != string::npos) {
            r1 = i;
            break;
        }
    }

    int r2 = rownum - 1;
    for (int i = rownum - 1; i >= 0; i--) {
        if (grove[i].find('#') != string::npos) {
            r2 = i;
            break;
        }
    }

    int c1 = 0;
    for (int i = 0; i < colnum; i++) {
        bool find = false;
        for (int j = 0; j < rownum; j++) {
            if (grove[j][i] == '#') {
                find = true;
                break;
            }
        }
        if (find) {
            c1 = i;
            break;
        }
    }

    int c2 = colnum - 1;
    for (int i = colnum - 1; i >= 0; i--) {
        bool find = false;
        for (int j = 0; j < rownum; j++) {
            if (grove[j][i] == '#') {
                find = true;
                break;
            }
        }
        if (find) {
            c2 = i;
            break;
        }
    }

    int count = 0;
    for (int i = r1; i <= r2; i++) {
        for (int j = c1; j <= c2; j++) {
            if (grove[i][j] == '.')
                count += 1;
        }
    }

    cout << count << endl;
}

void part2() {
    ifstream input("input");
    vector<string> grove;
    string line;
    while (getline(input, line)) {
        grove.push_back(line);
    }

    vector<array<pair<int, int>, 3>> dirs;

    dirs.push_back({{{-1, 0}, {-1, -1}, {-1, 1}}});
    dirs.push_back({{{1, 0}, {1, -1}, {1, 1}}});
    dirs.push_back({{{0, -1}, {-1, -1}, {1, -1}}});
    dirs.push_back({{{0, 1}, {-1, 1}, {1, 1}}});

    bool needmove = false;
    int round     = 0;
    while (true) {
        needmove = false;
        string emptyline(grove[0].size() + 2, '.');
        vector<string> newgrove;
        newgrove.push_back(emptyline);
        for (auto& s : grove) {
            string tmp(".");
            tmp.append(s);
            tmp.push_back('.');
            newgrove.push_back(tmp);
        }
        newgrove.push_back(emptyline);

        vector<pair<size_t, size_t>> from;
        vector<vector<pair<size_t, size_t>>> r(newgrove[0].size() + 2, from);
        vector<vector<vector<pair<size_t, size_t>>>> considers(newgrove.size() + 2, r);

        for (size_t r = 0; r < newgrove.size(); r++) {
            for (size_t c = 0; c < newgrove[0].size(); c++) {
                if (newgrove[r][c] == '#') {
                    auto mto = move_to(newgrove, dirs, r, c);
                    if (mto.first != static_cast<int>(r) || mto.second != static_cast<int>(c))
                        needmove = true;
                    // auto [a, b] = move_to(newgrove, dirs, r, c);
                    considers[mto.first][mto.second].push_back({r, c});
                }
            }
        }

        if (!needmove) {
            break;
        }

        vector<string> emptygrove(newgrove.size(), emptyline);

        for (size_t r = 0; r < newgrove.size(); r++) {
            for (size_t c = 0; c < newgrove[0].size(); c++) {
                if (considers[r][c].size() == 0) {
                    continue;
                } else if (considers[r][c].size() == 1) {
                    emptygrove[r][c] = '#';
                } else {
                    for (auto [a, b] : considers[r][c]) {
                        emptygrove[a][b] = '#';
                    }
                }
            }
        }

        grove = emptygrove;
        dirs.push_back(dirs[0]);
        dirs.erase(dirs.begin());
        round += 1;
        // cout << i << endl;
        // for (auto& s : grove) {
        //     for (auto& c : s) {
        //         cout << c;
        //     }
        //     cout << endl;
        // }
    }

    cout << round + 1 << endl;
}

int main() {
    part1();
    part2();
}