#include <iostream>

#include <algorithm>
#include <fstream>
#include <numeric>
#include <string>
#include <vector>

int main() {
    std::ifstream input("input");

    std::string line;

    std::vector<int> sums;
    std::vector<int> calories;

    // because the last line of input is empty,
    // if use while(getline(input, line)),
    // when read the last line ,the eof bit will set, stream operator bool return false,
    // the loop break immediately, then the last array of int not calculate
    while (input.good()) {
        std::getline(input, line);
        if (line.empty()) {
            auto sum = std::accumulate(calories.begin(), calories.end(), 0);
            sums.push_back(sum);
            calories.clear();
        } else {
            calories.push_back(std::stoi(line));
        }
    }

    std::sort(sums.begin(), sums.end());
    std::cout << *sums.rbegin() << std::endl;

    auto top3 = std::accumulate(sums.rbegin(), sums.rbegin() + 3, 0);

    std::cout << top3 << std::endl;
}