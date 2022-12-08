#include <algorithm>
#include <fstream>
#include <iostream>
#include <numeric>
#include <set>
#include <string>
#include <vector>

std::vector<int> check_row(const std::vector<char>& row) {
    std::vector<int> result;

    auto max_it = std::max_element(row.begin(), row.end() - 1);    // skip next edge

    while (max_it != row.begin()) {
        result.push_back(max_it - row.begin());
        max_it = std::max_element(row.begin(), max_it);
    }

    auto max_rit = std::max_element(row.rbegin(), row.rend() - 1);    // skip next edge
    while (max_rit != row.rbegin()) {
        result.push_back(row.size() - (max_rit - row.rbegin()) - 1);
        max_rit = std::max_element(row.rbegin(), max_rit);
    }
    return result;
}

std::vector<int> row_score(const std::vector<char>& row) {
    std::vector<int> scores(row.size(), 0);

    for (auto it = row.begin() + 1; it != row.end() - 1; it++) {    // skip edge

        auto height = *it;

        auto right_begin = it + 1;
        auto right       = std::find_if(right_begin, row.end(), [&](const auto& p) { return p >= height; });
        auto right_trees = right == row.end() ? right - right_begin : right - right_begin + 1;

        auto left_begin = row.rbegin() + (row.end() - it);
        auto left       = std::find_if(left_begin, row.rend(), [&](const auto& p) { return p >= height; });
        auto left_trees = left == row.rend() ? left - left_begin : left - left_begin + 1;

        scores[it - row.begin()] = right_trees * left_trees;
    }
    return scores;
}

int main() {
    std::ifstream input("input");
    std::string line;

    std::vector<std::vector<char>> treemap;
    while (std::getline(input, line)) {
        std::vector<char> tmp;
        for (auto& c : line) {
            tmp.push_back(c - '0');
        }
        treemap.push_back(tmp);
    }

    size_t row_num = treemap.size();
    size_t col_num = treemap[0].size();

    // part 1
    std::set<std::pair<int, int>> visiable_coord;

    for (size_t i = 1; i < row_num - 1; i++) {    // skip edge
        auto visiable_in_row = check_row(treemap[i]);
        for (auto j : visiable_in_row) {
            visiable_coord.insert({i, j});
        }
    }

    for (size_t i = 1; i < col_num - 1; i++) {
        std::vector<char> col_to_row;
        for (size_t j = 0; j < row_num; j++) {
            col_to_row.push_back(treemap[j][i]);
        }
        auto visiable_in_col = check_row(col_to_row);
        for (auto k : visiable_in_col) {
            visiable_coord.insert({k, i});
        }
    }

    std::cout << visiable_coord.size() + 2 * (row_num + col_num) - 4 << std::endl;

    // part 2
    std::vector<std::vector<int>> scores;

    for (auto& r : treemap) {
        scores.push_back(row_score(r));
    }

    for (size_t i = 0; i < col_num; i++) {
        std::vector<char> col_to_row;
        for (size_t j = 0; j < row_num; j++) {
            col_to_row.push_back(treemap[j][i]);
        }

        auto score_col = row_score(col_to_row);

        for (size_t j = 0; j < row_num; j++) {
            scores[j][i] = scores[j][i] * score_col[j];
        }
    }

    int max = 0;
    for (size_t i = 0; i < row_num; i++) {
        for (size_t j = 0; j < col_num; j++) {
            if (scores[i][j] > max) {
                max = scores[i][j];
            }
        }
    }

    std::cout << max << std::endl;
}