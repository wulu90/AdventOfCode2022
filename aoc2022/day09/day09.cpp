#include <fstream>
#include <functional>
#include <iostream>
#include <set>
#include <string>
#include <vector>

bool istouching(std::pair<int, int>& head, std::pair<int, int>& tail) {
    if (head == tail)
        return true;
    bool istouching = false;
    if (head.first == tail.first)    // same row
    {
        if (std::abs(head.second - tail.second) == 1)
            istouching = true;
    } else if (head.second == tail.second) {
        if (std::abs(head.first - tail.first) == 1)
            istouching = true;
    } else {
        if (std::abs(head.first - tail.first) == 1 && std::abs(head.second - tail.second) == 1)
            istouching = true;
    }

    return istouching;
}

void right(std::pair<int, int>& head, std::pair<int, int>& tail) {
    auto tmp = head;
    head.second += 1;
    if (!istouching(head, tail)) {
        tail = tmp;
    }
}

void left(std::pair<int, int>& head, std::pair<int, int>& tail) {
    auto tmp = head;
    head.second -= 1;
    if (!istouching(head, tail)) {
        tail = tmp;
    }
}

void up(std::pair<int, int>& head, std::pair<int, int>& tail) {
    auto tmp = head;
    head.first += 1;
    if (!istouching(head, tail)) {
        tail = tmp;
    }
}

void down(std::pair<int, int>& head, std::pair<int, int>& tail) {
    auto tmp = head;
    head.first -= 1;
    if (!istouching(head, tail)) {
        tail = tmp;
    }
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