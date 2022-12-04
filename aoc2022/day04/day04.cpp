#include <fstream>
#include <iostream>
#include <string>

struct range {
    int from;
    int to;
};

bool section_contain(std::pair<range, range>&& ranges) {
    auto [a, b] = ranges;
    return (a.from <= b.from && a.to >= b.to) || (a.from >= b.from && a.to <= b.to);
}

bool overlap_atall(std::pair<range, range>&& ranges) {
    auto [a, b] = ranges;
    return !(a.to < b.from || a.from > b.to);
}

bool section_contain(int a, int b, int c, int d) {
    return (a <= c && b >= d) || (a >= c && b <= d);
}

bool overlap_atall(int a, int b, int c, int d) {
    return !(b < c || a > d);
}

void part() {
    std::ifstream input("input");

    std::string line;
    int count_part1 = 0;
    int count_part2 = 0;

    while (std::getline(input, line)) {
        int first_dash = line.find_first_of('-');
        int comma      = line.find_first_of(',');
        int last_dash  = line.find_last_of('-');

        int a = std::stoi(line.substr(0, first_dash));
        int b = std::stoi(line.substr(first_dash + 1, comma - first_dash - 1));
        int c = std::stoi(line.substr(comma + 1, last_dash - comma - 1));
        int d = std::stoi(line.substr(last_dash + 1, line.size() - last_dash - 1));

        // if (section_contain(a, b, c, d)) {
        //     count_part1 += 1;
        // }

        // if (overlap_atall(a, b, c, d)) {
        //     count_part2 += 1;
        // }

        if (section_contain({{a, b}, {c, d}})) {
            count_part1 += 1;
        }

        if (overlap_atall({{a, b}, {c, d}})) {
            count_part2 += 1;
        }
    }

    std::cout << count_part1 << std::endl;
    std::cout << count_part2 << std::endl;
}

int main() {
    part();
}