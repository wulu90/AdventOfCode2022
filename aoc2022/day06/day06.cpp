#include <deque>
#include <fstream>
#include <iostream>
#include <set>
#include <string>

bool is_all_different(const std::deque<char>& cp, size_t num) {
    std::set char_set(cp.begin(), cp.end());
    if (char_set.size() == num)
        return true;
    else
        return false;
}

void decode_message(size_t diff_num) {
    std::ifstream input("input");

    std::deque<char> char_packet;
    char c;
    size_t count = 0;
    for (size_t i = 0; i < diff_num; i++) {
        input >> c;
        char_packet.push_back(c);
    }
    count += diff_num;
    if (is_all_different(char_packet, diff_num)) {
        std::cout << count << std::endl;
    } else {
        while (input >> c) {
            count += 1;
            char_packet.pop_front();
            char_packet.push_back(c);
            if (is_all_different(char_packet, diff_num)) {
                std::cout << count << std::endl;
                break;
            }
        }
    }
}

void part1() {
    decode_message(4);
}

void part2() {
    decode_message(14);
}

int main() {
    part1();
    part2();
}
