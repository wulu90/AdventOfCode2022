#include <algorithm>
#include <fstream>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

using namespace std;

struct node {
    int val;
    shared_ptr<node> prev;
    shared_ptr<node> next;

    node(int v) : val(v), prev(nullptr), next(nullptr) {}
};

void part1() {
    ifstream input("input");
    int val;
    string line;
    vector<int> ori;

    input >> val;
    ori.push_back(val);

    int index = 0;

    shared_ptr<node> lis   = make_shared<node>(index);
    shared_ptr<node> start = lis;
    while (input.good()) {
        input >> val;
        if (!input.eof()) {
            ori.push_back(val);
            index++;
            shared_ptr<node> tmp = make_shared<node>(index);
            tmp->prev            = lis;
            lis->next            = tmp;
            lis                  = tmp;
        }
    }

    auto zero_it   = find(ori.begin(), ori.end(), 0);
    int zero_index = distance(ori.begin(), zero_it);

    start->prev = lis;
    lis->next   = start;

    index = 0;

    int step    = 0;
    int orisize = ori.size();

    for (int i = 0; i < orisize; i++) {
        step = ori[i] % (orisize - 1);
        if (step == 0)
            continue;

        shared_ptr<node> tomove = start;
        while (tomove->val != i)
            tomove = tomove->next;

        tomove->prev->next   = tomove->next;
        tomove->next->prev   = tomove->prev;
        shared_ptr<node> tmp = tomove;

        step = step > 0 ? step : ori.size() + step - 1;
        while (step--) {
            tmp = tmp->next;
        }
        tmp->next->prev = tomove;
        tomove->next    = tmp->next;

        tomove->prev = tmp;
        tmp->next    = tomove;
    }

    shared_ptr<node> find = start;

    while (find->val != zero_index) {
        find = find->next;
    }

    for (int i = 0; i < 1000; i++) {
        find = find->next;
    }

    int n1000 = ori[find->val];

    for (int i = 0; i < 1000; i++) {
        find = find->next;
    }

    int n2000 = ori[find->val];

    for (int i = 0; i < 1000; i++) {
        find = find->next;
    }

    int n3000 = ori[find->val];

    cout << n1000 + n2000 + n3000 << endl;
}

int main() {
    part1();
}