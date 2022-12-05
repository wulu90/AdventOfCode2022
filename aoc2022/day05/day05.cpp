#include <deque>
#include <fstream>
#include <iostream>
#include <stack>
#include <string>
#include <vector>

#include <cstdio>

void build_stack(std::string& line, std::vector<std::deque<char>>& stacks) {
    size_t pos = 0;
    while ((pos = line.find('[', pos)) != std::string::npos) {
        stacks[pos / 4].push_back(line[pos + 1]);    // pattern like  "[A] " ,so divide by 4;

        pos += 3;
    }
}

void part1() {
    std::ifstream input("input");
    std::string line;

    std::vector<std::deque<char>> stacks;
    std::deque<char> ss;
    std::getline(input, line);

    int stack_size = (line.size() + 1) / 4;
    for (int i = 0; i < stack_size; i++) {
        stacks.push_back(ss);
    }
    build_stack(line, stacks);

    while (std::getline(input, line)) {
        if (line[1] == '1')
            break;

        build_stack(line, stacks);
    }

    std::getline(input, line);    // read blank line;

    while (std::getline(input, line)) {
        int crate_count, stack_from, stack_to;
        std::sscanf(line.c_str(), "move %d from %d to %d", &crate_count, &stack_from, &stack_to);

        for (int i = 0; i < crate_count; i++) {
            stacks[stack_to - 1].push_front(stacks[stack_from - 1].front());
            stacks[stack_from - 1].pop_front();
        }
    }

    for (size_t i = 0; i < stacks.size(); i++) {
        std::cout << stacks[i].front();
    }
    std::cout << std::endl;
}

void part2() {
    std::ifstream input("input");
    std::string line;

    std::vector<std::deque<char>> stacks;
    std::deque<char> ss;
    std::getline(input, line);

    int stack_size = (line.size() + 1) / 4;
    for (int i = 0; i < stack_size; i++) {
        stacks.push_back(ss);
    }
    build_stack(line, stacks);

    while (std::getline(input, line)) {
        if (line[1] == '1')
            break;

        build_stack(line, stacks);
    }

    std::getline(input, line);    // read blank line;

    while (std::getline(input, line)) {
        int crate_count, stack_from, stack_to;
        std::sscanf(line.c_str(), "move %d from %d to %d", &crate_count, &stack_from, &stack_to);

        // moved crates stay in the same order, first move to the back of deque then move front of the deque
        for (int i = 0; i < crate_count; i++) {
            stacks[stack_to - 1].push_back(stacks[stack_from - 1].front());
            stacks[stack_from - 1].pop_front();
        }
        for (int i = 0; i < crate_count; i++) {
            stacks[stack_to - 1].push_front(stacks[stack_to - 1].back());
            stacks[stack_to - 1].pop_back();
        }
    }

    for (size_t i = 0; i < stacks.size(); i++) {
        std::cout << stacks[i].front();
    }
    std::cout << std::endl;
}

int main() {
    part1();
    part2();
}
