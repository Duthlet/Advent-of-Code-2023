#include <vector>
#include <iostream>
#include <shortinttypes.h>
#include <string>
#include <set>
#include "input.inc"
#include <point.h>
#include <cassert>
#include <map>

std::set<point> rocks {};
std::set<point> cubes {};

#define SIZE 100

void parse_input () {
    rocks.clear();
    cubes.clear();
    for (i32 y = 0; y < SIZE; ++y)
        for (i32 x = 0; x < SIZE; ++x) {
            if (input[y][x] == '#') cubes.insert({y, x});
            if (input[y][x] == 'O') rocks.insert({y, x});
        }
};

auto tilt_north() {
    std::set<point> new_rocks {};
    for (auto p : rocks) {
        if (p.x == 0) {
            new_rocks.insert(p);
            continue;
        }
        while (1) {
            auto cand = p.left();
            if (new_rocks.contains(cand) || cubes.contains(cand) || (cand.x < 0)) {
                new_rocks.insert(p);
                break;
            }
            p = cand;
        }
    }
    assert (rocks.size() == new_rocks.size());
    return new_rocks;
}
auto tilt_south() {
    std::set<point> new_rocks {};
    for (auto it = rocks.rbegin(); it != rocks.rend(); ++it) {
        auto p = *it;
        if (p.x == SIZE-1) {
            new_rocks.insert(p);
            continue;
        }
        while (1) {
            auto cand = p.right();
            if (new_rocks.contains(cand) || cubes.contains(cand) || (cand.x >= SIZE)) {
                new_rocks.insert(p);
                break;
            }
            p = cand;
        }
    }
    assert (rocks.size() == new_rocks.size());
    return new_rocks;
}
auto tilt_east() {
    std::set<point> new_rocks {};
    for (auto it = rocks.rbegin(); it != rocks.rend(); ++it) {
        auto p = *it;
        if (p.y == SIZE-1) {
            new_rocks.insert(p);
            continue;
        }
        while (1) {
            auto cand = p.down();
            if (new_rocks.contains(cand) || cubes.contains(cand) || (cand.y >= SIZE)) {
                new_rocks.insert(p);
                break;
            }
            p = cand;
        }
    }
    assert (rocks.size() == new_rocks.size());
    return new_rocks;
}
auto tilt_west() {
    std::set<point> new_rocks {};
    for (auto p : rocks) {
        if (p.y == 0) {
            new_rocks.insert(p);
            continue;
        }
        while (1) {
            auto cand = p.up();
            if (new_rocks.contains(cand) || cubes.contains(cand) || (cand.y < 0)) {
                new_rocks.insert(p);
                break;
            }
            p = cand;
        }
    }
    assert (rocks.size() == new_rocks.size());
    return new_rocks;
}

u64 weight1() {
    u64 sum = 0;
    for (auto p: rocks) {
        assert (p.x >= 0);
        assert (p.x < SIZE);
        sum += SIZE - p.x;
    }
    return sum;
}

void print () {
    for (i32 y = 0; y < SIZE; ++y) {
        for (i32 x = 0; x < SIZE; ++x) {
            if (cubes.contains({y, x})) std::cout << ('#');
            else if (rocks.contains({y, x})) std::cout << ('O');
            else std::cout <<'.';
        }
        std::cout << "\n";
    }
}

u64 part1() {
    parse_input();
    rocks = tilt_north();
    return weight1();
}

void cycle () {
    rocks = tilt_north();
    rocks = tilt_west();
    rocks = tilt_south();
    rocks = tilt_east();
}

#define CYCLES 1000000000

u64 part2() {
    parse_input();
    std::map<std::set<point>, u64> cache;
    u64 i;
    cache.emplace(rocks, 0);
    for (i = 0; i < CYCLES; ++i) {
        cycle();
        auto f = cache.find(rocks);
        if (f == cache.end()) cache.emplace(rocks, i+1);
        else break;
    }
    if (i < CYCLES) {
        u64 diff = i+1 - cache[rocks];
        u64 missing = (CYCLES - i-1) % diff;
        for (i = 0; i < missing; ++i) {
            cycle();
        }
    }
    return weight1();
}

int main () {
    assert (input.size() == SIZE);
    assert (input[0].length() == SIZE);
    auto p1 = part1();
    std::cout << "Part 1: " << p1 << "\n";
    auto p2 = part2();
    std::cout << "Part 2: " << p2 << "\n";
    return 0;
}
