#include <algorithm>
#include <array>
#include <fstream>
#include <iostream>
#include <map>
#include <memory>
#include <queue>
#include <string>
#include <vector>

using namespace std;

struct valve {
    string label;
    int rate;
    vector<shared_ptr<valve>> leads;

    valve(const string& l, int r = 0) : label(l), rate(r), leads{} {}

    bool operator<(const valve& rhs) {
        return label < rhs.label;
    }
};

struct valve_op {
    char id;
    char rate;
    char leadnum;
    array<char, 5> leads;

    valve_op() : id(0), rate(0), leadnum(0) {}

    valve_op(char i) : id(i), rate(0), leadnum(0) {}

    valve_op(const valve_op& rhs) {
        id      = rhs.id;
        rate    = rhs.rate;
        leadnum = rhs.leadnum;
        leads   = rhs.leads;
    }

    bool operator<(const valve_op& rhs) {
        return id < rhs.id;
    }
};

struct node {
    shared_ptr<valve> curr;
    shared_ptr<valve> prev;
    vector<shared_ptr<valve>> opened;
    int pressures;

    node(const shared_ptr<valve>& v) : curr(v), opened(), pressures(0) {}
};

struct node_op {
    char curr;
    char prev;
    int64_t opened;
    int pressures;

    node_op(char cu, char pr = 0, int64_t op = 0, int press = 0) : curr(cu), prev(pr), opened(op), pressures(press) {}
};

void parse_valve(const string& line, map<string, shared_ptr<valve>>& valmap) {
    string l         = line.substr(6, 2);
    size_t semicolon = line.find(';');
    int r            = stoi(line.substr(23, semicolon - 23));

    if (!valmap.contains(l)) {
        valmap[l] = make_shared<valve>(valve{l});
    }
    valmap[l]->rate = r;

    size_t leadsinx = line[semicolon + 23] == 's' ? semicolon + 25 : semicolon + 24;
    string tostr;
    while (leadsinx < line.size()) {
        tostr = line.substr(leadsinx, 2);
        if (!valmap.contains(tostr)) {
            valmap[tostr] = make_shared<valve>(tostr);
        }
        valmap[l]->leads.push_back(valmap[tostr]);
        leadsinx += 4;
    }
}

void part1_op() {
    ifstream input("input");
    map<string, shared_ptr<valve>> valmap;

    string line;
    while (getline(input, line)) {
        parse_valve(line, valmap);
    }

    map<string, char> str_int;
    char count = 1;
    for (auto [k, v] : valmap) {
        str_int.insert({k, count});
        count++;
    }
    map<char, valve_op> valopmap;
    for (auto [k, v] : valmap) {
        valve_op vop(str_int[k]);
        vop.rate    = v->rate;
        vop.leadnum = v->leads.size();
        for (size_t i = 0; i < v->leads.size(); i++) {
            vop.leads[i] = str_int[v->leads[i]->label];
        }
        valopmap.insert({vop.id, vop});
    }
    queue<node_op> q;
    node_op a(str_int["AA"]);
    q.push(a);
    for (int i = 1; i < 30; i++) {
        cout << i << endl;
        size_t qsize = q.size();
        for (size_t j = 0; j < qsize; j++) {
            auto n = q.front();
            auto v = valopmap[n.curr];

            if (v.rate > 0 && (n.opened & (1L << v.id)) == 0) {
                node_op tmp(n.curr);
                tmp.pressures = n.pressures + v.rate * (30 - i);
                tmp.opened    = n.opened ^ (1L << v.id);
                q.push(tmp);
            }

            for (int k = 0; k < v.leadnum; k++) {
                if (n.prev != v.leads[k]) {
                    node_op tmp(v.leads[k]);
                    tmp.opened    = n.opened;
                    tmp.pressures = n.pressures;
                    tmp.prev      = n.curr;
                    q.push(tmp);
                }
            }
            q.pop();
        }
    }
    int max = 0;
    while (!q.empty()) {
        if (q.front().pressures > max)
            max = q.front().pressures;
        q.pop();
    }

    cout << max << endl;
}

void part1() {
    ifstream input("input");
    map<string, shared_ptr<valve>> valmap;

    string line;
    while (getline(input, line)) {
        parse_valve(line, valmap);
    }

    node a(valmap["AA"]);
    queue<node> q;
    q.push(a);

    for (int i = 1; i < 30; i++) {
        size_t qsize = q.size();
        cout << i << endl;
        for (size_t j = 0; j < qsize; j++) {
            auto n = q.front();

            if (n.curr->rate > 0 && find(n.opened.begin(), n.opened.end(), n.curr) == n.opened.end()) {
                node tmp(n.curr);
                tmp.opened = n.opened;
                tmp.opened.push_back(n.curr);
                tmp.pressures = n.pressures + n.curr->rate * (30 - i);
                q.push(tmp);
            }

            for (auto& v : n.curr->leads) {
                if (n.prev != v) {
                    node tmp(v);
                    tmp.opened    = n.opened;
                    tmp.pressures = n.pressures;
                    tmp.prev      = n.curr;
                    q.push(tmp);
                }
            }
            q.pop();
        }
    }
    int max = 0;
    while (!q.empty()) {
        if (q.front().pressures > max)
            max = q.front().pressures;
        q.pop();
    }

    cout << max << endl;
}

int main() {
    part1_op();
}