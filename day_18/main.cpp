#include <algorithm>
#include <vector>
#include <iostream>
#include <shortinttypes.h>
#include <string>
#include <cassert>
#include <set>
#include "point.h"

struct desc {
    char dir;
    u32 len;
    u32 color;
};

point move(point p, char dir) {
    switch(dir) {
        case 'L': return p.left();
        case 'R': return p.right();
        case 'U': return p.up();
        case 'D': return p.down();
        default: assert(false);
    }
}

#include "input.inc"

u64 part1() {
    std::vector<point> trenches;
    point min{0, 0};
    point max{0, 0};
    point cur {0,0};
    trenches.push_back(cur);
    for (const desc& d : input) {
        for (u32 i = 0; i < d.len; ++i) {
            cur = move(cur, d.dir);
            if (cur.x < min.x) min.x = cur.x;
            if (cur.x > max.x) max.x = cur.x;
            if (cur.y < min.y) min.y = cur.y;
            if (cur.y > max.y) max.y = cur.y;
            trenches.push_back(cur);
        }
    }
    std::cout << "Back: " << trenches.back() << "\n";
    trenches.pop_back();
    u64 dug = 0;
    bool dir_up;
    bool in_trench = false;
    for (i32 y = min.y; y <= max.y; ++y) {
        bool interior = false;
        for (i32 x = min.x; x <= max.x; ++x) {
            auto f = std::find(trenches.begin(), trenches.end(), point{x,y});
            if (f == trenches.end()) {
                dug += interior;
                continue;
            }
            ++dug;
            std::set<point> sur {};
            sur.insert(f != trenches.begin() ? *(f-1) : trenches.back());
            sur.insert(f+1 != trenches.end() ? *(f+1) : trenches.front());
            if (sur.contains(f->left()) && sur.contains(f->right())) continue;
            if (sur.contains(f->up()) && sur.contains(f->down())) {
                interior = !interior;
                continue;
            }
            if (!in_trench) {
                in_trench = true;
                if (sur.contains(f->up())) {
                    dir_up = true;
                } else
                if (sur.contains(f->down())) {
                    dir_up = false;
                }
                continue;
            }
            in_trench = false;
            if (sur.contains(f->up()) && !dir_up) interior = !interior;
            else if (sur.contains(f->down()) && dir_up) interior = !interior;
        }
    }
    return dug;
    return 0;
}

u64 part2() {
    return 0;
}

int main () {
    auto p1 = part1();
    std::cout << "Part 1: " << p1 << "\n";
    auto p2 = part2();
    std::cout << "Part 2: " << p2 << "\n";
    return 0;
}
