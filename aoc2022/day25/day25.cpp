#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <vector>
using namespace std;

void part1() {
    ifstream input("input");
    string line;
    vector<string> snafus;
    while (getline(input, line)) {
        snafus.push_back(line);
    }

    size_t maxsize = 0;

    for (auto& s : snafus) {
        if (s.size() > maxsize)
            maxsize = s.size();
    }

    map<char, int> smap{{'2', 2}, {'1', 1}, {'0', 0}, {'-', -1}, {'=', -2}};
    map<int, char> srmap{{2, '2'}, {1, '1'}, {0, '0'}, {-1, '-'}, {-2, '='}};
    int tmp = 0;
    string re_tmp;
    for (size_t i = 0; i < maxsize; i++) {
        for (auto& s : snafus) {
            if (i < s.size()) {
                tmp += smap[s[s.size() - i - 1]];
            }
        }

        int mod = tmp % 5;
        if (mod > 2) {
            mod = mod - 5;
            tmp = tmp / 5 + 1;
        } else {
            tmp = tmp / 5;
        }

        re_tmp.push_back(srmap[mod]);
    }

    // string result(re_tmp.rbegin(), re_tmp.rend());
    reverse(re_tmp.begin(), re_tmp.end());
    cout << re_tmp << endl;
}

int main() {
    part1();
}