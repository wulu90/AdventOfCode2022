
#include <fstream>
#include <iostream>
#include <ranges>
#include <set>
#include <string>
#include <vector>

using namespace std;

struct blueprint {
    int id;
    int ore_cost;
    int clay_cost;
    pair<int, int> obsidian_costs;
    pair<int, int> geode_costs;
};

struct state {
    // count of
    int ore;
    int clay;
    int obsidian;
    int geode;

    // count of robot
    int ore_robot;
    int clay_robot;
    int obsidian_robot;
    int geode_robot;

    auto operator<=>(const state& s) const = default;
};

blueprint parse_line(const string& line) {
    // Blueprint 7: Each ore robot costs 3 ore. Each clay robot costs 4 ore. Each obsidian robot costs 4 ore and 6 clay. Each geode robot
    // costs 2 ore and 20 obsidian.

    blueprint b;

    auto spview = line | std::views::split(':');    // split_view

    auto sr_id    = *spview.begin();             // id sub_range
    auto sr_costs = *next(spview.begin(), 1);    // costs sub_range

    auto sv1 = *(next((sr_id | std::views::split(' ')).begin(), 1));    // split_vew blueprint 1

    b.id = stoi(string(sv1.begin(), sv1.end()));

    auto ore_sp      = *((sr_costs | std::views::split('.')).begin()) | std::views::split(' ');    // Each,ore,robot,costs,4,ore
    auto sr_ore_cost = *next(ore_sp.begin(), 5);                                                   // subrange of id
    b.ore_cost       = stoi(string(sr_ore_cost.begin(), sr_ore_cost.end()));
    // cout << b.ore_cost << endl;

    auto clay_sp = *(next((sr_costs | std::views::split('.')).begin(), 1)) | std::views::split(' ');    // Each, clay, robot, costs, 4, ore
    auto tmp2    = *next(clay_sp.begin(), 5);                                                           //
    b.clay_cost  = stoi(string(tmp2.begin(), tmp2.end()));
    // cout << b.clay_cost << endl;

    // Each,obsidian,robot,costs,3,ore,and,11,clay
    auto obsi_sp     = *(next((sr_costs | std::views::split('.')).begin(), 2)) | std::views::split(' ');
    auto tmp3        = *next(obsi_sp.begin(), 5);
    auto tmp4        = *next(obsi_sp.begin(), 8);
    b.obsidian_costs = {stoi(string(tmp3.begin(), tmp3.end())), stoi(string(tmp4.begin(), tmp4.end()))};
    // cout << b.obsidian_costs.first << " " << b.obsidian_costs.second << endl;

    // Each,geode,robot,costs,3,ore,and,8,obsidian
    auto geod_sp  = *(next((sr_costs | std::views::split('.')).begin(), 3)) | std::views::split(' ');
    auto tmp5     = *next(geod_sp.begin(), 5);
    auto tmp6     = *next(geod_sp.begin(), 8);
    b.geode_costs = {stoi(string(tmp5.begin(), tmp5.end())), stoi(string(tmp6.begin(), tmp6.end()))};
    // cout << b.geode_costs.first << " " << b.geode_costs.second << endl;

    // cout << b.id << " " << b.ore_cost << " " << b.clay_cost << " " << b.obsidian_costs.first << " " << b.obsidian_costs.second << " "
    //      << b.geode_costs.first << " " << b.geode_costs.second << endl;
    return b;
}

void part1() {
    ifstream input("input");
    string line;

    vector<blueprint> bps;

    int total = 0;

    while (getline(input, line)) {
        bps.push_back(parse_line(line));
    }

    auto theoretical_max = [](int time_left) { return (time_left * (time_left - 1)) / 2; };

    for (auto& bp : bps) {
        int curr_max = 0;

        set<state> states;

        states.insert({0, 0, 0, 0, 1, 0, 0, 0});

        int ore_max = max(bp.ore_cost, bp.clay_cost);
        ore_max     = max(ore_max, bp.obsidian_costs.first);
        ore_max     = max(ore_max, bp.geode_costs.first);

        int clay_max = bp.obsidian_costs.second;
        int obsi_max = bp.geode_costs.second;

        for (int i = 1; i <= 24; i++) {
            set<state> new_state;

            int ore_max_consumed  = ore_max * (24 - i);
            int clay_max_consumed = clay_max * (24 - i);
            int obsi_max_consumed = obsi_max * (24 - i);

            for (auto& st : states) {
                if (st.geode + theoretical_max(24 - i) < curr_max) {
                    continue;
                }

                int ore_p  = st.ore_robot;
                int clay_p = st.clay_robot;
                int obsi_p = st.obsidian_robot;

                // build geode robot
                if (st.ore >= bp.geode_costs.first && st.obsidian >= bp.geode_costs.second) {
                    int ore_new_count  = st.ore - bp.geode_costs.first + ore_p;
                    int obsi_new_count = st.obsidian - bp.geode_costs.second + obsi_p;
                    int clay_new_count = st.clay + clay_p;

                    ore_new_count  = min(ore_new_count, ore_max_consumed);
                    clay_new_count = min(clay_new_count, clay_max_consumed);
                    obsi_new_count = min(obsi_new_count, obsi_max_consumed);

                    int geode_new_count = st.geode + (24 - i);
                    new_state.insert({
                        ore_new_count, clay_new_count, obsi_new_count, geode_new_count, st.ore_robot, st.clay_robot, st.obsidian_robot,
                        st.geode_robot,    // todo +1
                    });
                    curr_max = max(geode_new_count, curr_max);
                }

                // build ore robot
                if (st.ore_robot < ore_max && st.ore >= bp.ore_cost) {
                    int ore_count  = st.ore - bp.ore_cost + ore_p;
                    int clay_count = st.clay + clay_p;
                    int obsi_count = st.obsidian + obsi_p;

                    ore_count  = min(ore_count, ore_max_consumed);
                    clay_count = min(clay_count, clay_max_consumed);
                    obsi_count = min(obsi_count, obsi_max_consumed);

                    new_state.insert(
                        {ore_count, clay_count, obsi_count, st.geode, st.ore_robot + 1, st.clay_robot, st.obsidian_robot, st.geode_robot});
                }

                // build clay robot
                if (st.clay_robot < clay_max && st.ore >= bp.clay_cost) {
                    int ore_count  = st.ore - bp.clay_cost + ore_p;
                    int clay_count = st.clay + clay_p;
                    int obsi_count = st.obsidian + obsi_p;

                    ore_count  = min(ore_count, ore_max_consumed);
                    clay_count = min(clay_count, clay_max_consumed);
                    obsi_count = min(obsi_count, obsi_max_consumed);

                    new_state.insert(
                        {ore_count, clay_count, obsi_count, st.geode, st.ore_robot, st.clay_robot + 1, st.obsidian_robot, st.geode_robot});
                }

                // build obsidian robot
                if (st.obsidian_robot < obsi_max && st.ore >= bp.obsidian_costs.first && st.clay >= bp.obsidian_costs.second) {
                    int ore_count  = st.ore - bp.obsidian_costs.first + ore_p;
                    int clay_count = st.clay - bp.obsidian_costs.second + clay_p;
                    int obsi_count = st.obsidian + obsi_p;

                    ore_count  = min(ore_count, ore_max_consumed);
                    clay_count = min(clay_count, clay_max_consumed);
                    obsi_count = min(obsi_count, obsi_max_consumed);

                    new_state.insert(
                        {ore_count, clay_count, obsi_count, st.geode, st.ore_robot, st.clay_robot, st.obsidian_robot + 1, st.geode_robot});
                }

                int ore_count  = st.ore + ore_p;
                int clay_count = st.clay + clay_p;
                int obsi_count = st.obsidian + obsi_p;

                ore_count  = min(ore_count, ore_max_consumed);
                clay_count = min(clay_count, clay_max_consumed);
                obsi_count = min(obsi_count, obsi_max_consumed);

                new_state.insert(
                    {ore_count, clay_count, obsi_count, st.geode, st.ore_robot, st.clay_robot, st.obsidian_robot, st.geode_robot});
            }
            states = new_state;
        }

        total += curr_max * bp.id;
    }
    cout << total << endl;
}

void part2() {}

int main() {
    part1();
    // part2();
}
