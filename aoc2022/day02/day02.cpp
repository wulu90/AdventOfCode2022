#include <array>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

void part1() {
    std::ifstream input("input");

    // rock paper scissors
    // 1  2 3

    // outcome  oppo rock papar scissors
    // you rock     3 0 6
    // you paper    6 3 0
    // you scissors 0 6 3

    // add shape
    // 4 1 7
    // 8 5 2
    // 3 9 6

    std::array<int, 9> matrix{4, 1, 7, 8, 5, 2, 3, 9, 6};

    int sum = 0;
    std::string line;
    char strategy;
    while (std::getline(input, line)) {
        std::istringstream iss(line);
        iss >> strategy;
        int oppo_inx = strategy - 'A';
        iss >> strategy;
        int you_inx = strategy - 'X';

        sum += matrix[you_inx * 3 + oppo_inx];
    }

    std::cout << sum << std::endl;
}

void part2() {
    std::ifstream input("input");

    // rock paper scissors
    // 1  2 3
    // lose draw win
    // 0 3 6

    // op  choose       rock      paper      scissors
    // you lose  0 0 0  scissors  rock       paper
    // you draw  3 3 3  rock      paper      scissors
    // you win   6 6 6  paper     scissors   rock

    // add shape
    // 3 1 2
    // 4 5 6
    // 8 9 7

    std::array<int, 9> matrix{3, 1, 2, 4, 5, 6, 8, 9, 7};

    int sum = 0;
    std::string line;
    char strategy;
    while (std::getline(input, line)) {
        std::istringstream iss(line);
        iss >> strategy;
        int oppo_inx = strategy - 'A';
        iss >> strategy;
        int you_inx = strategy - 'X';

        sum += matrix[you_inx * 3 + oppo_inx];
    }

    std::cout << sum << std::endl;
}

int main() {
    part1();
    part2();
}