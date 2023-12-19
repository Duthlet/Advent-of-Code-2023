#include <algorithm>
#include <vector>
#include <iostream>
#include <shortinttypes.h>
#include <cassert>
#include <unordered_map>
#include <array>
#include "point.h"
#include "rows.h"

struct desc {
    char dir;
    u32 len;
    u32 color;
    void pt2() {
        switch (color % 4) {
            case 0: dir = 'R'; break;
            case 1: dir = 'D'; break;
            case 2: dir = 'L'; break;
            case 3: dir = 'U'; break;
        }
        len = color >> 4;
    }
};

#include "input.inc"

using row_t = row<i32, _interior>;

std::array<desc, 3> helper (u32 i) {
    std::array<desc, 3> retval;
    retval[0] = i == 0 ? input.back() : input[i-1];
    retval[1] = input[i];
    retval[2] = i == input.size() - 1 ? input.front() : input[i+1];
    return retval;
}

point insert (point p, std::array<desc, 3> d, std::unordered_map<i64, row_t>& rows) {
    switch(d[1].dir) {
        case 'L':
            rows[p.y].insert(p.x - i32(d[1].len), p.x, {d[0].dir == d[2].dir});
            return {p.x - i32(d[1].len), p.y};
        case 'R':
            rows[p.y].insert(p.x, p.x + i32(d[1].len), {d[0].dir == d[2].dir});
            return {p.x + i32(d[1].len), p.y};
        case 'U':
            for (u32 dy = 1; dy < d[1].len; ++dy) {
                rows[p.y - i32(dy)].insert(p.x, p.x, {true});
            }
            return {p.x, p.y - i32(d[1].len)};
        case 'D':
            for (u32 dy = 1; dy < d[1].len; ++dy) {
                rows[p.y + i32(dy)].insert(p.x, p.x, {true});
            }
            return {p.x, p.y + i32(d[1].len)};
        default: assert(false);
    }
}

u64 part1() {

    std::unordered_map<i64, row_t> rows;
    point cur {0,0};
    for (u32 i = 0; i < input.size(); ++i) {
        try {
            cur = insert(cur, helper(i), rows);
        }
        catch(...) {
            assert(false);
        }
    }
    u64 sum = 0;
    for (auto &r : rows) {
        try {
            sum += r.second.interior<bool>();
        }
        catch(...) {
            std::cout << "Assertion failed at row " << r.first << std::endl;
            assert(false);
        }
    }
    return sum;
}

u64 part2() {
    for (auto &d : input) d.pt2();
    return part1();
}

int main () {
/*
input = {
{'R', 6 ,0x70c710},
{'D', 5 ,0x0dc571},
{'L', 2 ,0x5713f0},
{'D', 2 ,0xd2c081},
{'R', 2 ,0x59c680},
{'D', 2 ,0x411b91},
{'L', 5 ,0x8ceee2},
{'U', 2 ,0xcaa173},
{'L', 1 ,0x1b58a2},
{'U', 2 ,0xcaa171},
{'R', 2 ,0x7807d2},
{'U', 3 ,0xa77fa3},
{'L', 2 ,0x015232},
{'U', 2 ,0x7a21e3}};
*/
    auto p1 = part1();
    std::cout << "Part 1: " << p1 << "\n";
    auto p2 = part2();
    std::cout << "Part 2: " << p2 << "\n";
    return 0;
}
