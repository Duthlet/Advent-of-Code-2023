#include <vector>
#include <iostream>
#include <shortinttypes.h>
#include <point.h>
#include <cassert>
#include <queue>
#include <unordered_set>

#include "input.inc"

char coord (const point& p) {
    if ((p.y < 0) || (p.y >= int(input.size())) || (p.x < 0) || (p.x >= int(input[p.y].length()))) return '.';
    return input[p.y][p.x];
}

point find_start() {
    for (int y = 0; y < int(input.size()); ++y)
        for (int x = 0; x < int(input[y].length()); ++x)
            if (coord({x, y}) == 'S') return {x, y};
    assert(false);
}

bool legal_start(point from, point pos) {
    switch (coord(pos)) {
        case '.': return false;
        case '|': return from == pos.up() || from == pos.down();
        case '-': return from == pos.left() || from == pos.right();
        case '7': return from == pos.left() || from == pos.down();
        case 'F': return from == pos.right() || from == pos.down();
        case 'L': return from == pos.up() || from == pos.right();
        case 'J': return from == pos.up() || from == pos.left();
        default: assert (false);
    }
}

point next_tile(const point& prev, const point& cur) {
    switch (coord(cur)) {
        case '|': return prev == cur.down() ? cur.up() : cur.down();
        case '-': return prev == cur.right() ? cur.left() : cur.right();
        case '7': return prev == cur.down() ? cur.left() : cur.down();
        case 'F': return prev == cur.down() ? cur.right() : cur.down();
        case 'L': return prev == cur.up() ? cur.right() : cur.up();
        case 'J': return prev == cur.up() ? cur.left() : cur.up();
        default:
            std::cerr << cur << " contains " << coord(cur) << "\n";
            assert (false);
    }
}

std::vector<point> part1() {
    auto start = find_start();

    std::cout << "Start: " << start << "\n";
    std::vector<point> loop{};
    loop.push_back(start);

    std::vector<point> second{start.left(), start.right(), start.up(), start.down()};
    for (const auto& p : second) {
        if (legal_start(start, p)) {
            loop.push_back(p);
            break;
        }
    }

    auto in_loop = [&](const point& pos) {
        for (const auto& l : loop)
            if (l == pos) return true;
        return false;
    };

    assert (loop.size() == 2);
    while (1) {
        auto next = next_tile(*(loop.end()-2), *(loop.end()-1));
        if (in_loop(next)) break;
        loop.push_back(next);
    }

    return loop;
}

bool in_grid (const point& p) {
    return !((p.x % 2) || (p.y % 2));
}

bool in_bounds (const point& p) {
    return ( (p.y >= -1) && (p.y <= 2*int(input.size()) - 1)
          && (p.x >= -1) && (p.x <= 2*int(input.size()) - 1));
}

point to_point (u64 p) {
    return { i32(u32(p << 32)), i32(u32(p)) };
}

int part2(std::vector<point>& loop_v) {
    std::unordered_set<u64> loop {};
    for (auto &t : loop_v) loop.insert(u64(2*t));

    std::cout << "Size(loop) = " << loop.size() << "\n";

    std::queue<point> check {};
    for (int y = -1; y <= 2*int(input.size()) - 1; ++y) {
        check.push({-1, y});
        check.push({2*int(input[0].length()) - 1, y});
    }
    for (int x = 0; x < 2*int(input[0].length()) - 1; ++x) {
        check.push({x, -1});
        check.push({x, 2*int(input.size())-1});
    }

    auto in_loop = [&](const point& p) {
        if (in_grid(p)) {
            return bool(loop.count(u64(p)));
        }
        if (loop.count(u64(p.left())) && loop.count(u64(p.right()))) {
            char left = coord({(p.x-1)/2, p.y/2});
            char right = coord({(p.x+1)/2, p.y/2});
            if ((left == '-'
              || left == 'L'
              || left == 'F'
              || left == 'S')
             && (right == '-'
              || right == 'J'
              || right == '7'
              || right == 'S'))
                return true;
            return false;
        }
        if (loop.count(u64(p.up())) && loop.count(u64(p.down()))) {
            char up = coord({p.x/2, (p.y-1)/2});
            char down = coord({p.x/2, (p.y+1)/2});
            if ((up == '|'
              || up == '7'
              || up == 'F'
              || up == 'S')
             && (down == '|'
              || down == 'J'
              || down == 'L'
              || down == 'S'))
                return true;
            return false;
        }
        return false;
    };

    std::unordered_set<u64> checked {};
    while (!check.empty()) {
        point tbc = check.front();
        check.pop();
        if (in_bounds(tbc) && (!checked.count(u64(tbc))) && (!in_loop(tbc))) {
            checked.insert(u64(tbc));
            check.push(tbc.right());
            check.push(tbc.left());
            check.push(tbc.down());
            check.push(tbc.up());
        }
    }

    u64 sum = 0;
    for (auto u : checked) {
        sum += in_grid(to_point(u));
    }
    return 140*140 - sum - loop.size();
}

int main () {
    auto p1 = part1();
    std::cout << "Part 1: " << p1.size() / 2 << "\n";
    auto p2 = part2(p1);
    std::cout << "Part 2: " << p2 << "\n";
    return 0;
}
