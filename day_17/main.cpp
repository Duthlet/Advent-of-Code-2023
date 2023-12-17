#include <vector>
#include <iostream>
#include <shortinttypes.h>
#include <queue>
#include <string>
#include <map>
#include <cassert>
#include "point.h"
#include "input.inc"

int lb;
int ub;

struct info {
    u16 dir;
    u16 count;

    friend auto operator<=>(const info&, const info&) = default;
};

struct node {
    point p;
    info i;
    u64 min;
};

bool operator<(const node& lhs, const node& rhs) {
    if (lhs.min > rhs.min) return true;
    if (lhs.min < rhs.min) return false;
    if (lhs.p < rhs.p) return true;
    if (lhs.p > rhs.p) return false;
    if (lhs.i < rhs.i) return true;
    return false;
}

#define LEFT 1
#define RIGHT 2
#define UP 4
#define DOWN 8

#define SIZE 141

point move (const point p, const info i) {
    switch (i.dir) {
        case LEFT:
            return p.left();
        case RIGHT:
            return p.right();
        case UP:
            return p.up();
        case DOWN:
            return p.down();
        default:
            assert(false);
    }
}

std::vector<info> next(const info i) {
    std::vector<info> retval {};
    if (i.count >= lb) {
    switch (i.dir) {
        case LEFT:
            retval.emplace_back(UP, 1);
            retval.emplace_back(DOWN, 1);
            break;
        case RIGHT:
            retval.emplace_back(UP, 1);
            retval.emplace_back(DOWN, 1);
            break;
        case UP:
            retval.emplace_back(LEFT, 1);
            retval.emplace_back(RIGHT, 1);
            break;
        case DOWN:
            retval.emplace_back(LEFT, 1);
            retval.emplace_back(RIGHT, 1);
            break;
        default:
            assert(false);
    }}
    if (i.count < ub) retval.emplace_back(i.dir, i.count+1);
    return retval;
}

using x = std::map<info,u64>;

x cache[SIZE][SIZE];

std::priority_queue<node> to_check;

bool in_bounds(point p) {
    return (p.x >= 0)
        && (p.x < SIZE)
        && (p.y >= 0)
        && (p.y < SIZE);
}

bool check(node& cur) {
    // check
    auto &c = cache[cur.p.y][cur.p.x];
    auto f = c.find(cur.i);
    if (f == c.end()) {
        c.emplace(cur.i, cur.min);
//        std::cout << "Setting " << cur.p << " direction " << cur.i.dir
//                  << " count " << cur.i.count << " to " << cur.min << "\n";
        if (cur.p == point{SIZE-1, SIZE-1}) return true;
    } else {
        if (cur.min < f->second) {
            f->second = cur.min;
//        std::cout << "Updating " << cur.p << " direction " << cur.i.dir
//                  << " count " << cur.i.count << " to " << cur.min << "\n";
        }
        else {
//        std::cout << "Rejecting " << cur.p << " direction " << cur.i.dir
//                  << " count " << cur.i.count << " at " << cur.min << "\n";
            return false;
        }
    }
    // inseert new nodes
    auto ni = next(cur.i);
    for (auto i : ni) {
        point p = move(cur.p, i);
        if (!in_bounds(p)) continue;
        u64 min = cur.min + (input[p.y][p.x] - '0');
        to_check.emplace(p, i, min);
    }
    return false;
}

u64 part1() {
    lb = 0;
    ub = 3;
    to_check.emplace(point{0,0}, info{LEFT, 0}, 0);
    to_check.emplace(point{0,0}, info{UP, 0}, 0);
    node _next;
    do {
        assert(!to_check.empty());
        _next = to_check.top();
        to_check.pop();
    } while(!check(_next));
    return cache[SIZE-1][SIZE-1].begin()->second;
}

u64 part2() {
    for (u32 y = 0; y < SIZE; ++y)
        for (u32 x = 0; x < SIZE; ++x)
            cache[y][x] = {};
    to_check = {};
    lb = 4;
    ub = 10;
    to_check.emplace(point{0,0}, info{LEFT, 10}, 0);
    to_check.emplace(point{0,0}, info{UP, 10}, 0);
    node _next;
    do {
        assert(!to_check.empty());
        _next = to_check.top();
        to_check.pop();
    } while(!check(_next));
    return cache[SIZE-1][SIZE-1].begin()->second;
}

int main () {
/*
input = {
"2413432311323",
"3215453535623",
"3255245654254",
"3446585845452",
"4546657867536",
"1438598798454",
"4457876987766",
"3637877979653",
"4654967986887",
"4564679986453",
"1224686865563",
"2546548887735",
"4322674655533"};
*/
    assert(SIZE == input.size());
    assert(SIZE == input[0].length());
    auto p1 = part1();
    std::cout << "Part 1: " << p1 << "\n";
    auto p2 = part2();
    std::cout << "Part 2: " << p2 << "\n";
    return 0;
}
