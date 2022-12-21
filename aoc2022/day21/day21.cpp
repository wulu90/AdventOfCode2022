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
    char op;
    bool status;
    shared_ptr<target> dep1;
    shared_ptr<target> dep2;

    target(const string& s) : id(s) {}
};

function<long(long, long)> getfunc(char c) {
    if (c == '+')
        return std::plus<long>();
    else if (c == '-')
        return std::minus<long>();
    else if (c == '*')
        return std::multiplies<long>();
    else
        return std::divides<long>();
}

function<long(long, long)> getfunc_inverse(char c) {
    if (c == '-')
        return std::plus<long>();
    else if (c == '+')
        return std::minus<long>();
    else if (c == '/')
        return std::multiplies<long>();
    else
        return std::divides<long>();
}

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
        t->op     = line[11];
    }
}

long calc(const shared_ptr<target>& t) {
    if (t->status) {
        return t->val;
    } else {
        long l    = calc(t->dep1);
        long r    = calc(t->dep2);
        t->val    = getfunc(t->op)(l, r);
        t->status = true;
        return t->val;
    }
}

bool has_humn(const shared_ptr<target>& t) {
    if (t->id == "humn")
        return true;
    else {
        if (!t->status) {
            return has_humn(t->dep1) || has_humn(t->dep2);
        } else
            return false;
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

void part2() {
    ifstream input("input");
    string line;
    map<string, shared_ptr<target>> targetmap;
    while (getline(input, line)) {
        parse_target(line, targetmap);
    }

    long value = has_humn(targetmap["root"]->dep1) ? calc(targetmap["root"]->dep2) : calc(targetmap["root"]->dep1);

    shared_ptr<target> curr = has_humn(targetmap["root"]->dep1) ? targetmap["root"]->dep1 : targetmap["root"]->dep2;

    while (curr->id != "humn") {
        bool left_humn = has_humn(curr->dep1) ? true : false;
        long correct   = left_humn ? calc(curr->dep2) : calc(curr->dep1);

        if (curr->op == '+' || curr->op == '*')
            value = getfunc_inverse(curr->op)(value, correct);
        else {
            if (left_humn)
                value = getfunc_inverse(curr->op)(value, correct);
            else
                value = getfunc(curr->op)(correct, value);
        }

        curr = left_humn ? curr->dep1 : curr->dep2;
    }

    cout << value << endl;

    targetmap["humn"]->val = value;
    cout << calc(targetmap["root"]->dep1) << endl;
    cout << calc(targetmap["root"]->dep2) << endl;
}

int main() {
    part1();
    part2();
}