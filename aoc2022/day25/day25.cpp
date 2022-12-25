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
    size_t maxsize = 0;
    while (getline(input, line)) {
        snafus.push_back(line);
        if (line.size() > maxsize)
            maxsize = line.size();
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
        } else if (mod >= -2 && mod <= 2) {
            tmp = tmp / 5;
        } else {
            mod = mod + 5;
            tmp = tmp / 5 - 1;
        }
        re_tmp.push_back(srmap[mod]);
    }

    string result(re_tmp.rbegin(), re_tmp.rend());
    cout << result << endl;
}

int main() {
    part1();
}