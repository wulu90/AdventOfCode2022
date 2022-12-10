#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

int main() {
    std::ifstream input("input");

    std::string line;

    std::vector<int> during_value;
    std::string instruction;
    int oprand;
    std::istringstream iss;

    int init_value = 1;
    while (std::getline(input, line)) {
        iss.str(line);
        iss >> instruction;
        if (instruction == "addx") {
            iss >> oprand;
            during_value.push_back(init_value);
            during_value.push_back(init_value);
            init_value += oprand;
        } else {
            during_value.push_back(init_value);
        }
        iss.clear();
    }

    int signal_strenghts = 0;
    for (int i = 19; i < 220; i += 40) {
        signal_strenghts += (i + 1) * during_value[i];
    }

    std::cout << signal_strenghts << std::endl;

    for (int i = 0; i < 6; i++) {
        for (int j = 0; j < 40; j++) {
            if (j >= during_value[i * 40 + j] - 1 && j <= during_value[i * 40 + j] + 1) {
                std::cout << '#';
            } else {
                std::cout << '.';
            }
        }
        std::cout << std::endl;
    }
}