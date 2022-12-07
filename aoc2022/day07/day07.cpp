#include <algorithm>
#include <fstream>
#include <functional>
#include <iostream>
#include <map>
#include <memory>
#include <numeric>
#include <sstream>
#include <stack>
#include <string>
#include <vector>

enum class command { cd, ls };

struct file {
    std::string name;
    size_t size;
};

struct dir {
    std::string name;
    std::string absolute_name;
    size_t size;
    std::shared_ptr<dir> parent_dir;
    std::vector<std::shared_ptr<dir>> subdirs;
    std::vector<file> files;
};

command read_command(const std::string& line) {
    if (line[2] == 'c' && line[3] == 'd')
        return command::cd;
    else
        return command::ls;
}

std::string read_change_dir(const std::string& line) {
    return std::string(line.begin() + 5, line.end());
}

void change_dir(std::shared_ptr<dir>& curr_dir, const std::string& to_dir) {
    if (to_dir == "..") {
        curr_dir = curr_dir->parent_dir;
    } else {
        curr_dir = *std::find_if(curr_dir->subdirs.begin(), curr_dir->subdirs.end(), [&](auto& p) { return p->name == to_dir; });
    }
}

void add_subdir(std::shared_ptr<dir>& curr_dir, const std::string& line) {
    auto sub_dir           = std::make_shared<dir>();
    sub_dir->name          = std::string(line.begin() + 4, line.end());
    sub_dir->absolute_name = curr_dir->absolute_name + sub_dir->name + "/";
    sub_dir->size          = 0;
    sub_dir->parent_dir    = curr_dir;
    curr_dir->subdirs.push_back(sub_dir);
}

void edit_dir_size(std::shared_ptr<dir>& curr_dir, size_t size) {
    curr_dir->size += size;
    while (curr_dir->parent_dir != nullptr) {
        curr_dir = curr_dir->parent_dir;
        edit_dir_size(curr_dir, size);
    }
}

void add_file(std::shared_ptr<dir>& curr_dir, const std::string& line) {
    std::istringstream iss(line);
    size_t size;
    std::string name;
    iss >> size;
    iss >> name;
    file f{name, size};
    curr_dir->files.push_back(f);

    auto temp = curr_dir;
    edit_dir_size(curr_dir, size);
    curr_dir = temp;
}

std::vector<size_t> walk_tree(const std::shared_ptr<dir>& root, std::function<bool(std::shared_ptr<dir>&)> f) {
    std::stack<std::shared_ptr<dir>> ds;
    std::vector<size_t> result;
    ds.push(root);
    while (!ds.empty()) {
        auto dir = ds.top();
        ds.pop();
        if (f(dir)) {
            result.push_back(dir->size);
        }
        for (auto& d : dir->subdirs)
            ds.push(d);
    }

    return result;
}

void solve() {
    std::ifstream input("input");

    auto root           = std::make_shared<dir>();
    root->name          = "/";
    root->absolute_name = "/";
    root->size          = 0;
    root->parent_dir    = nullptr;
    auto curr_dir       = root;
    std::string line;
    while (std::getline(input, line)) {
        if (line[0] == '$') {    // command

            auto comm = read_command(line);
            if (comm == command::cd) {
                auto to_dir = read_change_dir(line);
                if (to_dir == "/") {
                    curr_dir = root;
                } else {
                    change_dir(curr_dir, to_dir);
                }
            } else if (comm == command::ls) {
                continue;
            }

        } else if (line.find("dir") == 0) {    // dir name
            add_subdir(curr_dir, line);
        } else {    // file  size name
            add_file(curr_dir, line);
        }
    }

    // part1
    auto less100K_vec   = walk_tree(root, [](std::shared_ptr<dir>& d) { return d->size <= 100000; });
    auto total_less100K = std::accumulate(less100K_vec.begin(), less100K_vec.end(), 0);

    // part2
    size_t disk_size = 70000000;
    size_t target    = 30000000;

    size_t need_size     = target - (disk_size - root->size);
    auto to_del_vec      = walk_tree(root, [&](std::shared_ptr<dir>& d) { return d->size >= need_size; });
    auto smallest_to_del = std::min_element(to_del_vec.begin(), to_del_vec.end());

    std::cout << total_less100K << std::endl;
    std::cout << *smallest_to_del << std::endl;
}

int main() {
    solve();
}