#include <fstream>
#include <functional>
#include <iostream>
#include <map>
#include <memory>
#include <string>
#include <vector>

using namespace std;

struct target {
    string id;
    long val;
    function<long(long, long)> op;
    bool status;
    shared_ptr<target> dep1;
    shared_ptr<target> dep2;

    target(const string& s) : id(s) {}
};

void parse_target(const string& line, map<string, shared_ptr<target>>& targetmap) {
    string id = line.substr(0, 4);

    if (!targetmap.contains(id)) {
        targetmap.insert({id, make_shared<target>(id)});
    }
    shared_ptr<target> t = targetmap[id];
    if (line[6] >= '0' && line[6] <= '9') {
        t->val    = stoi(line.substr(6, line.size() - 6));
        t->status = true;
    } else {
        string d1 = line.substr(6, 4);
        string d2 = line.substr(13, 4);
        if (!targetmap.contains(d1)) {
            targetmap.insert({d1, make_shared<target>(d1)});
        }
        if (!targetmap.contains(d2)) {
            targetmap.insert({d2, make_shared<target>(d2)});
        }
        t->dep1   = targetmap[d1];
        t->dep2   = targetmap[d2];
        t->status = false;
        if (line[11] == '+')
            t->op = std::plus<long>();
        else if (line[11] == '-')
            t->op = std::minus<long>();
        else if (line[11] == '*')
            t->op = std::multiplies<long>();
        else if (line[11] == '/')
            t->op = std::divides<long>();
    }
}

long calc(const shared_ptr<target>& t) {
    if (t->status) {
        return t->val;
    } else {
        long l    = calc(t->dep1);
        long r    = calc(t->dep2);
        t->val    = t->op(l, r);
        t->status = true;
        return t->val;
    }
}

void part1() {
    ifstream input("input");
    string line;
    map<string, shared_ptr<target>> targetmap;
    while (getline(input, line)) {
        parse_target(line, targetmap);
    }

    auto num = calc(targetmap["root"]);
    cout << num << endl;
}

int main() {
    part1();
}