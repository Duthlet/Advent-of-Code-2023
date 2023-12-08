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

int next_dir (int &i) {
    int retval = dirs[i];
    i = (i + 1) % dirs.size();
    return retval;
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

bool done () {
    std::sort (start2.begin(), start2.end());

    for (int i = 0; i< start2.size(); ++i) {
        if (start2[i] != goal2[i]) return false;
    }
    return true;
}

int at_goal (int i) {
    for (int n = 0; n < goal2.size(); ++n)
        if (i == goal2[n]) return n;
    return -1;
}

std::pair<u32, int> blubb (int i, int start) {
    u32 retval = 1;
    i = move[next_dir(start)][i];
    int n;
    while ((n = at_goal(i)) < 0) {
        i = move[next_dir(start)][i];
        ++retval;
    }
    return {retval, n};
}

std::vector<std::pair<u32, int>> bla (int i) {
    std::vector<std::pair<u32, int>> next {};
    for (int start = 0; start < dirs.size(); ++start) {
        next.push_back(blubb(i, start));
    }
    return next;
}

std::vector<std::vector <std::pair<u32, int>>> nextz {};
struct ghost {
    ghost (int start) {
        auto p = blubb(start, 0);
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
        //auto p = blubb(goal2[to],steps % dirs.size());
        auto p = nextz[to][steps % dirs.size()];
        steps += p.first;
        to = p.second;
        int i;
        return steps;
    }

};

u64 part2() {
    for (int i : goal2) {
        nextz.push_back(bla(i));
    }
    std::cout << "calculated nextz\n";
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

void parse_input() {
#include "input.inc"
/*std::string directions = "LR";

std::unordered_map<std::string, std::pair<std::string, std::string>> input {
{"11A",{"11B","11B"}},
{"11B",{"11B","11Z"}},
{"11Z",{"11B","11B"}},
{"22A",{"22B","11B"}},
{"22B",{"22C","22C"}},
{"22C",{"22Z","22Z"}},
{"22Z",{"22B","22B"}},
{"XXX",{"11B","11B"}}};
*/
    std::unordered_map<std::string, int> indices;
    int i = 0;
    for (const auto & line : input) indices.emplace(line.first, i++);

    std::cout << "indices:\n"; 
    for (const auto & x : indices) {
        std::cout << x.first << ", " << x.second << ", " << input[x.first].first << ", " << input[x.first].second << "\n";
    }
    std::cout << "endindices:\n"; 

    for (char c : directions) dirs.push_back(c == 'L' ? 0 : 1);

    std::unordered_map<int, int> left;
    std::unordered_map<int, int> right;
    start1 = indices["AAA"];
    goal1 = indices["ZZZ"];
    for (const auto& line : input) {
        int i = indices[line.first];
        left.emplace(i, indices[line.second.first]);
        right.emplace(i, indices[line.second.second]);
        if (line.first[2] == 'A') start2.push_back(i);
        if (line.first[2] == 'Z') goal2.push_back(i);
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
