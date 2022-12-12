#include <algorithm>
#include <array>
#include <fstream>
#include <iostream>
#include <queue>
#include <set>
#include <string>
#include <vector>

using namespace std;

array<pair<int, int>, 4> dir{{{0, -1}, {1, 0}, {0, 1}, {-1, 0}}};    // left,down,right,up

vector<pair<int, int>> find_next_BFS(const vector<string>& heightmap, const pair<int, int>& curr, const set<pair<int, int>>& visited) {
    vector<pair<int, int>> re;
    int rownum = heightmap.size();
    int colnum = heightmap.begin()->size();
    for (auto [a, b] : dir) {
        auto row = curr.first + a;
        auto col = curr.second + b;
        if (row < 0 || row >= rownum || col < 0 || col >= colnum)
            continue;

        if (visited.find({row, col}) == visited.end()) {
            auto from = heightmap[curr.first][curr.second];
            auto to   = heightmap[row][col];
            if (to - from <= 1)
                re.push_back({row, col});
        }
    }
    return re;
}

vector<pair<int, int>> find_next_BFS_2(const vector<string>& heightmap, const pair<int, int>& curr, const set<pair<int, int>>& visited) {
    vector<pair<int, int>> re;
    int rownum = heightmap.size();
    int colnum = heightmap.begin()->size();
    for (auto [a, b] : dir) {
        auto row = curr.first + a;
        auto col = curr.second + b;
        if (row < 0 || row >= rownum || col < 0 || col >= colnum)
            continue;

        if (visited.find({row, col}) == visited.end()) {
            auto from = heightmap[curr.first][curr.second];
            auto to   = heightmap[row][col];
            if (from - to <= 1)
                re.push_back({row, col});
        }
    }
    return re;
}

void findpath_BFS(const vector<string>& heightmap, const pair<int, int>& start, const pair<int, int>& end) {
    set<pair<int, int>> visited;
    queue<pair<pair<int, int>, int>> q;    // coord , step
    q.push({start, 0});

    while (q.front().first != end) {
        auto& item = q.front();
        q.pop();

        for (auto p : find_next_BFS(heightmap, item.first, visited)) {
            visited.insert(p);
            q.push({p, item.second + 1});
        }
    }

    std::cout << q.front().second << std::endl;
}

void findpath_BFS_2(const vector<string>& heightmap, const pair<int, int>& start) {
    set<pair<int, int>> visited;
    queue<pair<pair<int, int>, int>> q;    // coord , step
    q.push({start, 0});

    while (heightmap[q.front().first.first][q.front().first.second] != 'a') {
        auto& item = q.front();
        q.pop();

        for (auto p : find_next_BFS_2(heightmap, item.first, visited)) {
            visited.insert(p);
            q.push({p, item.second + 1});
        }
    }

    std::cout << q.front().second << std::endl;
}

void part1() {
    ifstream input("input");
    vector<string> heightmap;
    string line;

    while (getline(input, line)) {
        heightmap.push_back(line);
    }

    pair<int, int> start, end;
    size_t i = 0, j = 0;
    bool find_s = false, find_e = false;
    for (auto& s : heightmap) {
        if ((j = s.find('S')) != string::npos) {
            s[j]   = 'a';
            start  = {i, j};
            find_s = true;
        }
        if ((j = s.find('E')) != string::npos) {
            s[j]   = 'z';
            end    = {i, j};
            find_e = true;
        }
        if (find_s && find_e)
            break;
        i += 1;
    }

    vector<pair<int, int>> path;
    vector<vector<pair<int, int>>> path_all;

    findpath_BFS(heightmap, start, end);
}

void part2() {
    ifstream input("input");
    vector<string> heightmap;
    string line;

    while (getline(input, line)) {
        heightmap.push_back(line);
    }

    pair<int, int> start, end;
    size_t i = 0, j = 0;
    bool find_s = false, find_e = false;
    for (auto& s : heightmap) {
        if ((j = s.find('S')) != string::npos) {
            s[j]   = 'a';
            start  = {i, j};
            find_s = true;
        }
        if ((j = s.find('E')) != string::npos) {
            s[j]   = 'z';
            end    = {i, j};
            find_e = true;
        }
        if (find_s && find_e)
            break;
        i += 1;
    }

    vector<pair<int, int>> path;
    vector<vector<pair<int, int>>> path_all;

    findpath_BFS_2(heightmap, end);
}

int main() {
    part1();
    part2();
}

// some DFS code
// will find all path, two slow
// findpath(start, path, heightmap, path_all, end);

// path.push_back(start);
// findpath2(path, heightmap, path_all, end);

// path_all = findpath_DFS(heightmap, start, end);

// sort(path_all.begin(), path_all.end(), [](auto& p1, auto& p2) { return p1.size() < p2.size(); });

// std::cout << path_all.begin()->size() - 1 << std::endl;

void findpath(pair<int, int> tofind, vector<pair<int, int>> path, vector<string>& heightmap, vector<vector<pair<int, int>>>& path_all,
              pair<int, int>& end) {
    path.push_back(tofind);
    if (tofind == end) {
        path_all.push_back(path);
        return;
    }
    int rownum = heightmap.size();
    int colnum = heightmap[0].size();
    for (size_t i = 0; i < dir.size(); i++) {
        auto row = tofind.first + dir[i].first;
        auto col = tofind.second + dir[i].second;

        if (row < 0 || row >= rownum || col < 0 || col >= colnum)
            continue;
        if (find(path.begin(), path.end(), make_pair(row, col)) == path.end()) {
            if (heightmap[row][col] <= heightmap[tofind.first][tofind.second] ||
                heightmap[row][col] == heightmap[tofind.first][tofind.second] + 1) {
                findpath({row, col}, path, heightmap, path_all, end);
            }
        }
    }
}

vector<pair<int, int>> find_next(const vector<string>& heightmap, const vector<pair<int, int>>& path, const set<pair<int, int>>& visited) {
    vector<pair<int, int>> re;
    int rownum = heightmap.size();
    int colnum = heightmap.begin()->size();
    for (auto [a, b] : dir) {
        auto row = path.back().first + a;
        auto col = path.back().second + b;
        if (row < 0 || row >= rownum || col < 0 || col >= colnum)
            continue;

        if (visited.find({row, col}) == visited.end()) {
            if (heightmap[row][col] <= heightmap[path.back().first][path.back().second] ||
                heightmap[row][col] == heightmap[path.back().first][path.back().second] + 1)
                re.push_back({row, col});
        }
    }
    return re;
}

vector<vector<pair<int, int>>> findpath_DFS(const vector<string>& heightmap, const pair<int, int>& start, const pair<int, int>& end) {
    vector<vector<pair<int, int>>> path_all;

    vector<vector<pair<int, int>>> choices_vec;

    vector<pair<int, int>> path;
    set<pair<int, int>> visited;
    path.push_back(start);
    visited.insert(start);

    choices_vec.push_back(find_next(heightmap, path, visited));

    while (!path.empty()) {
        visited.clear();
        copy(path.begin(), path.end(), inserter(visited, visited.begin()));
        auto& choices = choices_vec.back();
        if (!choices.empty()) {
            auto next = choices.back();
            path.push_back(next);
            visited.insert(next);
            choices.pop_back();
            auto next_choice = find_next(heightmap, path, visited);
            choices_vec.push_back(next_choice);

        } else {
            path.pop_back();
            choices_vec.pop_back();
        }

        if (path.back() == end) {
            path_all.push_back(path);
            path.pop_back();
            choices_vec.pop_back();
        }
    }

    return path_all;
}

void findpath2(vector<pair<int, int>> path, vector<string>& heightmap, vector<vector<pair<int, int>>>& path_all, pair<int, int>& end) {
    auto last = path[path.size() - 1];
    if (last == end) {
        path_all.push_back(path);
        return;
    }
    int rownum = heightmap.size();
    int colnum = heightmap[0].size();
    for (size_t i = 0; i < dir.size(); i++) {
        auto row = last.first + dir[i].first;
        auto col = last.second + dir[i].second;

        if (row < 0 || row >= rownum || col < 0 || col >= colnum)
            continue;
        if (find(path.begin(), path.end(), make_pair(row, col)) == path.end()) {
            if (heightmap[row][col] <= heightmap[last.first][last.second] ||
                heightmap[row][col] == heightmap[last.first][last.second] + 1) {
                // path.push_back({row, col});
                // findpath2(path, heightmap, path_all, end);
                // why need a copy??
                auto p = path;
                p.push_back({row, col});
                findpath2(p, heightmap, path_all, end);
            }
        }
    }
}