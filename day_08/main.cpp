#include <vector>
#include <iostream>
#include <unordered_map>
#include <shortinttypes.h>

#include "input.inc"

char next_dir () {
    static int i = 0;
    char retval = directions[i];
    i = (i + 1) % directions.length();
    return retval;
}

u64 part1() {
    std::string cur = "AAA";
    u64 steps = 0;

    while (cur != "ZZZ") {
        ++steps;
        switch(next_dir()) {
            case 'L':
                cur = input[cur].first;
                break;
            case 'R':
                cur = input[cur].second;
                break;
            default:
                throw 1;
        }
    }

    return steps;
}

bool done (std::vector<std::string> v) {
    for (const auto &s : v)
        if (s[2] != 'Z') return false;
    return true;
}

std::vector<u64> hit (const std::string& str) {
    std::vector<u64> retval{};
    std::string cur = str;
    u64 i = 0;
    u64 ord = 0;
    do {
        ++ord;
        for (char c : directions) {
            ++i;
            switch(c) {
                case 'L':
                    cur = input[cur].first;
                    break;
                case 'R':
                    cur = input[cur].second;
                    break;
                default:
                    throw 1;
            }
            if (cur[2] == 'Z') retval.push_back(i);
        }
    } while (cur != str);
    retval.push_back(ord);
    return retval;
}

struct ghost {
    std::vector<u64> v;
    int i;
    u64 last;

    u64 next () {
        int mod = i % (v.size() - 1);
        int rem = i / (v.size() - 1);
        ++i;
        last = v[mod] + rem*v.back();
        return last;
    }

    bool check (u64 cand) {
        while(1) {
            if (last == cand) return true;
            if (last < cand) next();
            if (last > cand) return false;
        }
    }

    ghost (std::vector<u64> _v) : i(0), last(0), v(_v) { }
};
/*
u64 next (const std::vector<u64> v, int i) {
    return v[i % (v.size() - 1)] + (i / (v.size() - 1)) * v.back();
}
*/
int part2() {
    std::vector<ghost> ghosts {};

    for (const auto& line : input) {
        if (line.first[2] == 'A') {
            ghosts.emplace_back(hit(line.first));
        }
    }

    while (1) {
        u64 cand = ghosts[0].next();
        std::cout << "new candidate " << cand << "\n";
        bool hit = true;
        for (int j = 1; j < ghosts.size(); ++j) {
            if (!ghosts[j].check(cand)) {
                hit = false;
                break;
            }
        }
        if (hit) return cand;
    }
}

int main () {
//    auto p1 = part1();
    auto p2 = part2();
  //  std::cout << "Part 1: " << p1 << "\n";
    std::cout << "Part 2: " << p2 << "\n";
    return 0;
}
