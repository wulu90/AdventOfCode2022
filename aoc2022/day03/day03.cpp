#include <algorithm>
#include <fstream>
#include <iostream>
#include <set>
#include <string>

void part1() {
    std::ifstream input("input");

    std::string line;

    int sum = 0;

    while (std::getline(input, line)) {
        std::set<char> first_half;
        for (size_t i = 0; i < line.size() / 2; i++) {
            first_half.insert(line[i]);
        }
        char item_type;
        for (size_t i = line.size() / 2; i < line.size(); i++) {
            if (first_half.find(line[i]) != first_half.end()) {
                item_type = line[i];
                break;
            }
        }

        if (std::isupper(item_type)) {
            sum += item_type - 'A' + 27;
        } else {
            sum += item_type - 'a' + 1;
        }
    }

    std::cout << sum << std::endl;
}

void gen_set(std::set<char>& char_set, const std::string& line) {
    for (auto& c : line) {
        char_set.insert(c);
    }
}

void part2() {
    std::ifstream input("input");

    std::string line;

    int sum = 0;

    while (!input.eof()) {
        std::set<char> set1, set2, set3;
        std::set<char> set_tmp1, set_all;

        std::getline(input, line);
        if (line.empty())
            break;
        gen_set(set1, line);

        std::getline(input, line);
        gen_set(set2, line);

        std::getline(input, line);
        gen_set(set3, line);

        std::set_intersection(set1.begin(), set1.end(), set2.begin(), set2.end(), std::inserter(set_tmp1, set_tmp1.begin()));

        std::set_intersection(set3.begin(), set3.end(), set_tmp1.begin(), set_tmp1.end(), std::inserter(set_all, set_all.begin()));

        char group_type = *set_all.begin();

        if (std::isupper(group_type)) {
            sum += group_type - 'A' + 27;
        } else {
            sum += group_type - 'a' + 1;
        }
    }

    std::cout << sum << std::endl;
}

int main() {
    part1();
    part2();
}
