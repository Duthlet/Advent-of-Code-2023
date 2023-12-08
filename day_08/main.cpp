#include <vector>
#include <iostream>
#include <unordered_map>
#include <shortinttypes.h>
#include <algorithm>

std::vector<int> dirs;
int start1;
int goal1;
std::vector<int> start2;
std::vector<int> goal2;
std::vector<std::vector <int>> move;
std::unordered_map<u64, std::pair<u32, int>> nexty {};

int next_dir (int &i) {
    int retval = dirs[i];
    i = (i + 1) % dirs.size();
    return retval;
}

struct ghost {

    static std::pair<u32, int> find_next_step_and_goal_index (int i, int start) {
        u32 retval = 1;
        i = move[next_dir(start)][i];
        int n;

        auto at_goal = [&]() {
            for (n = 0; n < goal2.size(); ++n)
                if (i == goal2[n]) return n;
            n = -1;
            return n;
        };

        while (at_goal() < 0) {
            i = move[next_dir(start)][i];
            ++retval;
        }
        return {retval, n};
    }

    ghost (int start) {
        auto p = find_next_step_and_goal_index(start, 0);
        steps = p.first;
        to = p.second;
    }

    u64 steps;
    int to;

    bool check(u64 cand) {
        while (1) {
            if (cand == steps) return true;
            if (cand < steps) return false;
            next();
        }
    }
    u64 next() {
        auto x = std::pair{to, steps % dirs.size()};
        u64 i = (u64(x.first) << 32) + u64(x.second);
        auto f = nexty.find(i);
        if (f == nexty.end()) {
            nexty[i] = find_next_step_and_goal_index(goal2[x.first],x.second);
        }
        auto p = nexty[i];
        steps += p.first;
        to = p.second;
        return steps;
    }

};

u64 part2() {
    std::vector<ghost> ghosts {};
    for (int i : start2) {
        ghosts.emplace_back(i);
    }

    u64 steps = 0;
    int i = 0;
    while (1) {
        steps = ghosts[0].steps;
        bool done = true;
        for (auto &g : ghosts) {
            if (!g.check(steps)) {
                done = false;
                break;
            }
        }
        if (done) break;
        ghosts[0].next();
    }
    return steps;
}

u64 part1() {
    u64 steps = 0;
    int i = 0;
    while (start1 != goal1) {
        ++steps;
        start1 = move[next_dir(i)][start1];
    }
    return steps;
}

void parse_input() {
#include "input.inc"

    std::unordered_map<std::string, int> indices;
    int i = 0;
    for (const auto & line : input) indices.emplace(line[0], i++);

    for (char c : directions) dirs.push_back(c == 'L' ? 0 : 1);

    std::unordered_map<int, int> left;
    std::unordered_map<int, int> right;
    start1 = indices["AAA"];
    goal1 = indices["ZZZ"];
    for (const auto& line : input) {
        int i = indices[line[0]];
        left.emplace(i, indices[line[1]]);
        right.emplace(i, indices[line[2]]);
        if (line[0][2] == 'A') start2.push_back(i);
        if (line[0][2] == 'Z') goal2.push_back(i);
    }
    std::sort(goal2.begin(), goal2.end());
    move.push_back(std::vector<int>{});
    move.push_back(std::vector<int>{});
    for (i = 0; i < left.size(); ++i) {
        move[0].push_back(left[i]);
        move[1].push_back(right[i]);
    }
    std::cout << "starts: ";
    for (auto i : start2) std::cout << i << ",";
    std::cout << "\ngoals: ";
    for (auto i : goal2) std::cout << i << ",";
}

int main () {
    parse_input();
    std::cout << "done parsing\n";
    auto p1 = part1();
    std::cout << "Part 1: " << p1 << "\n";
    auto p2 = part2();
    std::cout << "Part 2: " << p2 << "\n";
    return 0;
}
