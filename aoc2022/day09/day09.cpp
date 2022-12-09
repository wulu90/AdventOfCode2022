#include <array>
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

bool check_touching_and_moving(std::pair<int, int>& head, std::pair<int, int>& tail) {
    if (head != tail) {
        if (head.first == tail.first)    // same row
        {
            if (std::abs(head.second - tail.second) > 1) {
                tail.second += head.second > tail.second ? 1 : -1;
                return false;
            }
            return true;
        } else if (head.second == tail.second) {
            if (std::abs(head.first - tail.first) > 1) {
                tail.first += head.first > tail.first ? 1 : -1;
                return false;
            }
            return true;
        } else {
            if (!(std::abs(head.first - tail.first) == 1 && std::abs(head.second - tail.second) == 1)) {
                tail.first += head.first > tail.first ? 1 : -1;
                tail.second += head.second > tail.second ? 1 : -1;
                return false;
            }
            return true;
        }
    }
    return true;
}

void right(std::pair<int, int>& head) {
    head.second += 1;
}

void left(std::pair<int, int>& head) {
    head.second -= 1;
}

void up(std::pair<int, int>& head) {
    head.first += 1;
}

void down(std::pair<int, int>& head) {
    head.first -= 1;
}

std::function<void(std::pair<int, int>& head)> move_func(char d) {
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
        f(head);
        check_touching_and_move(head, tail);
        tailvisited.insert(tail);
    }
}

void move(char d, int step, std::array<std::pair<int, int>, 10>& knots, std::set<std::pair<int, int>>& tailvisited) {
    auto f = move_func(d);
    while (step--) {
        f(knots[0]);
        for (int i = 1; i < 10; i++) {
            // if two knot is touching, the knots after that are touching, the loop can break;
            if (check_touching_and_moving(knots[i - 1], knots[i]))
                break;
        }
        tailvisited.insert(knots[9]);
    }
}

void part1() {
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

void part2() {
    std::ifstream input("input");

    std::string line;

    std::array<std::pair<int, int>, 10> knots;
    std::set<std::pair<int, int>> tailvisited;
    tailvisited.insert(knots[9]);
    char direction;
    int step;
    while (std::getline(input, line)) {
        direction = line[0];
        step      = std::stoi(std::string(line.begin() + 2, line.end()));

        move(direction, step, knots, tailvisited);
    }

    std::cout << tailvisited.size() << std::endl;
}

int main() {
    part1();
    part2();
}