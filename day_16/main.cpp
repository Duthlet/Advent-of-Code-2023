#include <vector>
#include <iostream>
#include <shortinttypes.h>
#include <map>
#include <unordered_map>
#include <cassert>
#include <string>
#include <queue>
#include <point.h>

#include "input.inc"

std::unordered_map<point, u8> energized {};
std::queue<std::pair<point, u8>> q {};

#define LEFT 1
#define RIGHT 2
#define UP 4
#define DOWN 8

bool in_bounds(point p) {
    return (p.x >= 0)
        && (p.x < i64(input[0].length()))
        && (p.y >= 0)
        && (p.y < i64(input.size()));
}

char ch(point p) {
    return input[p.y][p.x];
}

void trace (std::pair<point, u8>& p) {
    while(in_bounds(p.first)) {
        auto f = energized.find(p.first);
        if (f != energized.end()) {
            if (f->second & p.second) break;
            f->second |= p.second;
        } else {
            energized.insert(p);
        }
        switch (ch(p.first)) {
            case '/' :
                p.second =
                std::map<u8, u8> {
                    {LEFT, DOWN},
                    {RIGHT, UP},
                    {UP, RIGHT},
                    {DOWN, LEFT}
                }[p.second];
                goto straight;
            case '\\':
                p.second =
                std::map<u8, u8> {
                    {LEFT, UP},
                    {RIGHT, DOWN},
                    {UP,LEFT},
                    {DOWN, RIGHT}
                }[p.second];
                goto straight;
            case '-' :
                if (p.second & (LEFT | RIGHT)) goto straight;
                q.push({p.first.left(), LEFT});
                p.second = RIGHT;
                goto straight;
            case '|' :
                if (p.second & (UP | DOWN)) goto straight;
                q.push({p.first.up(), UP});
                p.second = DOWN;
                goto straight;
            case '.' :
            straight:
                switch (p.second) {
                    case LEFT: p.first = p.first.left(); break;
                    case RIGHT: p.first = p.first.right(); break;
                    case UP: p.first = p.first.up(); break;
                    case DOWN: p.first = p.first.down(); break;
                    default: assert (false);
                }
                break;
            default:
                assert (false);
        }
    }
}

u64 energized_tiles(std::pair<point, u8> start) {
    q = {};
    energized.clear();
    q.push(start);
    while(!q.empty()) {
        trace(q.front());
        q.pop();
    }
    return energized.size();
}

u64 part1() {
    return energized_tiles({{0,0}, RIGHT});
}

u64 part2() {
    u64 cand;
    u64 max = 0;
    std::vector<u64> energies {};
    for (int x = 0; x < int(input[0].length()); ++x) {
        cand = energized_tiles({{x,0}, DOWN});
        if (cand > max) max = cand;
        cand = energized_tiles({{x,int(input.size())-1}, UP});
        if (cand > max) max = cand;
    }
    for (int y = 0; y < int(input.size()); ++y) {
        cand = energized_tiles({{0,y}, RIGHT});
        if (cand > max) max = cand;
        cand = energized_tiles({{int(input[0].length())-1,y}, LEFT});
        if (cand > max) max = cand;
    }
    return max;
}

int main () {
    auto p1 = part1();
    std::cout << "Part 1: " << p1 << "\n";
    auto p2 = part2();
    std::cout << "Part 2: " << p2 << "\n";
    return 0;
}
