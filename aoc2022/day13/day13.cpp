#include <algorithm>
#include <fstream>
#include <iostream>
#include <numeric>
#include <string>
#include <variant>
#include <vector>

using namespace std;

struct node {
    variant<int, vector<node>> data;

    explicit node(int i) : data(i) {}

    explicit node(vector<node> v) : data(move(v)) {}

    friend bool operator<(const node& lhs, const node& rhs) {
        if (lhs.data.index() == 0 && rhs.data.index() == 0) {
            return get<int>(lhs.data) < get<int>(rhs.data);
        } else if (lhs.data.index() == 1 && rhs.data.index() == 1) {
            return get<vector<node>>(lhs.data) < get<vector<node>>(rhs.data);
        } else {
            if (lhs.data.index() == 0 && rhs.data.index() == 1) {
                return vector<node>{node{get<int>(lhs.data)}} < get<vector<node>>(rhs.data);
            } else {
                return get<vector<node>>(lhs.data) < vector<node>{node{get<int>(rhs.data)}};
            }
        }
    }
};

int parse_int(string::iterator& s, string::iterator e) {
    return stoi(string(s, e));
}

vector<node> parse_list(string::iterator& s, string::iterator e) {
    vector<node> v;

    s++;    // [
    while (s != e && *s != ']') {
        if (isdigit(*s)) {
            auto it = find_if(s, e, [](char c) { return c == ',' || c == ']'; });
            v.emplace_back(parse_int(s, it));
            s = it;
        } else if (*s == '[') {
            v.emplace_back(parse_list(s, e));
            s++;
        }
        if (*s == ',') {
            s++;
        }
    }
    return v;
}

node parse_node(string& line) {
    auto s = line.begin();
    auto e = line.end();
    return node{parse_list(s, e)};
}

void part1() {
    ifstream input("input");
    string line;
    vector<node> nv;
    while (getline(input, line)) {
        if (!line.empty())
            nv.emplace_back(parse_node(line));
    }

    vector<size_t> right;
    for (size_t i = 0; i < nv.size() / 2; i++) {
        if (nv[i * 2] < nv[i * 2 + 1])
            right.push_back(i + 1);
    }

    cout << accumulate(right.begin(), right.end(), 0) << endl;
}

void part2() {
    ifstream input("input");
    string line;
    vector<node> nv;
    while (getline(input, line)) {
        if (!line.empty())
            nv.emplace_back(parse_node(line));
    }
    sort(nv.begin(), nv.end());
    string str = "[[2]]";
    node n1    = parse_node(str);
    str        = "[[6]]";
    node n2    = parse_node(str);
    auto it1   = find_if_not(nv.begin(), nv.end(), [&](auto& p) { return p < n1; });
    auto it2   = find_if_not(nv.begin(), nv.end(), [&](auto& p) { return p < n2; });
    cout << (it1 - nv.begin() + 1) * (it2 - nv.begin() + 2) << endl;
}

int main() {
    part1();
    part2();
}
