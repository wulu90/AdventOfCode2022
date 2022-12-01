#include <iostream>

#include <fstream>
#include <string>
#include <vector>
#include <numeric>
#include <algorithm>

#include <queue>

int main()
{

    std::ifstream input("/home/wl/Desktop/AdventOfCode2022/aoc2022/day01/input");

    std::string line;

    std::vector<int> sums;
    std::vector<int> calories;

    while (std::getline(input, line))
    {
        if ((!input.eof()&&line.empty())||input.eof())
        {
            auto sum = std::accumulate(calories.begin(), calories.end(), 0);
            sums.push_back(sum);
            calories.clear();
        }
        else
        {
            calories.push_back(std::stoi(line));
        }
    }

    auto most = std::max_element(sums.begin(), sums.end());

    std::cout << *most << std::endl;

    std::priority_queue<int> pq(sums.begin(),sums.end());

    int top=0;
    for(int i=0;i<3;i++){
        top+=pq.top();
        pq.pop();
    }

    std::cout << top << std::endl;

}