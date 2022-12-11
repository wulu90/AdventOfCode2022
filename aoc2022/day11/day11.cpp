#include <deque>
#include <fstream>
#include <functional>
#include <iostream>
#include <string>
#include <vector>

std::function<int64_t(int64_t)> parse_operation(const std::string& line) {
    auto old_inx = line.find("old");
    std::function<int64_t(int64_t)> f;
    int64_t v;
    std::string oprand = line.substr(old_inx + 6, line.size() - old_inx - 6);
    if (line[old_inx + 4] == '*') {
        if (oprand == "old") {
            f = [](int64_t p) { return p * p; };
        } else {
            v = std::stol(oprand);
            f = [v](int64_t p) { return p * v; };
        }
    } else {
        v = std::stol(oprand);
        f = [v](int64_t p) { return p + v; };
    }
    return f;
}

int64_t parse_test(const std::string& line) {
    auto last_space = line.find_last_of(' ');
    return std::stoi(line.substr(last_space + 1, line.size() - last_space - 1));
}

std::deque<int64_t> parse_starting_items(const std::string& line) {
    auto colon_inx = line.find(':');

    size_t comma_inx;
    auto start = colon_inx + 2;
    std::deque<int64_t> result;

    while (start < line.size()) {
        comma_inx = line.find(',', start);
        if (comma_inx != std::string::npos) {
            result.push_back(std::stol(line.substr(start, comma_inx - start)));
            start = comma_inx + 2;
        } else {
            result.push_back(std::stol(line.substr(start, line.size() - start)));
            break;
        }
    }

    return result;
}

size_t parse_throwto(const std::string& line) {
    auto last_space = line.find_last_of(' ');
    return std::stol(line.substr(last_space + 1, line.size() - last_space - 1));
}

struct Monkey {
    std::deque<int64_t> items;
    std::function<int64_t(int64_t)> operation;
    int64_t testv;
    size_t true_to;
    size_t false_to;
    int64_t inspect_time = 0;

    void inpect(std::vector<Monkey>& monkeys) {
        while (!items.empty()) {
            inspect_time += 1;
            auto item = items.front();
            items.pop_front();
            auto afterop = operation(item);
            afterop      = afterop / 3;
            size_t to    = afterop % testv == 0 ? true_to : false_to;
            monkeys[to].items.push_back(afterop);
        }
    }

    void inspect2(std::vector<Monkey>& monkeys, int64_t prime_mod) {
        while (!items.empty()) {
            inspect_time += 1;
            auto item = items.front();
            items.pop_front();
            auto afterop = operation(item);

            afterop   = afterop % prime_mod;
            size_t to = afterop % testv == 0 ? true_to : false_to;
            monkeys[to].items.push_back(afterop);
        }
    }
};

Monkey parse_monkey(std::ifstream& input) {
    Monkey m;
    std::string line;
    std::getline(input, line);
    std::getline(input, line);
    m.items = parse_starting_items(line);
    std::getline(input, line);
    m.operation = parse_operation(line);
    std::getline(input, line);
    m.testv = parse_test(line);
    std::getline(input, line);
    m.true_to = parse_throwto(line);
    std::getline(input, line);
    m.false_to = parse_throwto(line);
    std::getline(input, line);    // read empty line for sepereate;
    return m;
}

void part1() {
    std::ifstream input("input");

    std::string line;
    std::vector<Monkey> monkeys;

    while (!input.eof()) {
        Monkey m = parse_monkey(input);
        monkeys.push_back(m);
    }

    for (int i = 0; i < 20; i++) {
        for (auto& m : monkeys) {
            m.inpect(monkeys);
        }
    }

    std::vector<int64_t> insptectime_vec;
    for (auto& m : monkeys) {
        insptectime_vec.push_back(m.inspect_time);
    }

    std::sort(insptectime_vec.begin(), insptectime_vec.end());

    std::cout << *insptectime_vec.rbegin() * *(insptectime_vec.rbegin() + 1) << std::endl;
}

void part2() {
    std::ifstream input("input");

    std::string line;
    std::vector<Monkey> monkeys;

    while (!input.eof()) {
        Monkey m = parse_monkey(input);
        monkeys.push_back(m);
    }

    int64_t prime_mod = 1;
    for (auto& m : monkeys) {
        prime_mod *= m.testv;
    }

    for (int i = 0; i < 10000; i++) {
        for (auto& m : monkeys) {
            m.inspect2(monkeys, prime_mod);
        }
    }

    std::vector<int64_t> insptectime_vec;
    for (auto& m : monkeys) {
        insptectime_vec.push_back(m.inspect_time);
    }

    std::sort(insptectime_vec.begin(), insptectime_vec.end());
    auto business = *insptectime_vec.rbegin() * *(insptectime_vec.rbegin() + 1);
    std::cout << business << std::endl;
}

int main() {
    part1();
    part2();
}