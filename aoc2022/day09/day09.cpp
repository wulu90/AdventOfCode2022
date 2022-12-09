#include <fstream>
#include <functional>
#include <iostream>
#include <set>
#include <string>
#include <vector>

void check_touching_and_move(std::pair<int, int>& head, std::pair<int, int>& tail) {
    if (head != tail) {
        if (head.first == tail.first)    // same row
        {
            if (std::abs(head.second - tail.second) > 1) {
                tail.second += head.second > tail.second ? 1 : -1;
            }
        } else if (head.second == tail.second) {
            if (std::abs(head.first - tail.first) > 1)
                tail.first += head.first > tail.first ? 1 : -1;
        } else {
            if (!(std::abs(head.first - tail.first) == 1 && std::abs(head.second - tail.second) == 1)) {
                tail.first += head.first > tail.first ? 1 : -1;
                tail.second += head.second > tail.second ? 1 : -1;
            }
        }
    }
}

void right(std::pair<int, int>& head, std::pair<int, int>& tail) {
    head.second += 1;
    check_touching_and_move(head, tail);
}

void left(std::pair<int, int>& head, std::pair<int, int>& tail) {
    head.second -= 1;
    check_touching_and_move(head, tail);
}

void up(std::pair<int, int>& head, std::pair<int, int>& tail) {
    head.first += 1;
    check_touching_and_move(head, tail);
}

void down(std::pair<int, int>& head, std::pair<int, int>& tail) {
    head.first -= 1;
    check_touching_and_move(head, tail);
}

std::function<void(std::pair<int, int>& head, std::pair<int, int>& tail)> move_func(char d) {
    if (d == 'R')
        return right;
    else if (d == 'L')
        return left;
    else if (d == 'U')
        return up;
    else
        return down;
}

void move(char d, int step, std::pair<int, int>& head, std::pair<int, int>& tail, std::set<std::pair<int, int>>& tailvisited) {
    auto f = move_func(d);
    while (step--) {
        f(head, tail);
        tailvisited.insert(tail);
    }
}

int main() {
    std::ifstream input("input");

    std::string line;

    std::pair<int, int> head(0, 0);
    std::pair<int, int> tail(0, 0);
    std::set<std::pair<int, int>> tailvisited;
    tailvisited.insert(tail);
    char direction;
    int step;
    while (std::getline(input, line)) {
        direction = line[0];
        step      = std::stoi(std::string(line.begin() + 2, line.end()));

        move(direction, step, head, tail, tailvisited);
    }

    std::cout << tailvisited.size() << std::endl;
}