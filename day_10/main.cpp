#include <vector>
#include <iostream>
#include <shortinttypes.h>
#include <cassert>
#include <queue>
#include <unordered_set>

#include "input.inc"

char coord (int x, int y) {
    if ((y < 0) || (y >= int(input.size())) || (x < 0) || (x >= int(input[y].length()))) return '.';
    return input[y][x];
}

std::pair<int, int> find_start() {
    for (int y = 0; y < int(input.size()); ++y)
        for (int x = 0; x < int(input[y].length()); ++x)
            if (coord(x, y) == 'S') return {x, y};
    assert(false);
}

struct loop_tile {
    int x;
    int y;
    int from_x;
    int from_y;
    size_t distance;
    bool legal_start();
    loop_tile next_tile();
};

bool loop_tile::legal_start() {
    std::cerr << x << "," << y << " contains " << coord(x,y) << "\n";
    switch (coord(x, y)) {
        case '.': return false;
        case '|': return (from_x==x) && (from_y==y - 1 || from_y==y + 1);
        case '-': return (from_y==y) && (from_x==x - 1 || from_x==x + 1);
        case '7': return (from_x==x-1 && from_y==y)||(from_x==x && from_y==y+1);
        case 'F': return (from_x==x+1 && from_y==y)||(from_x==x && from_y==y+1);
        case 'L': return (from_x==x+1 && from_y==y)||(from_x==x && from_y==y-1);
        case 'J': return (from_x==x-1 && from_y==y)||(from_x==x && from_y==y-1);
        default:
        assert (false);
    }
}

loop_tile loop_tile::next_tile() {
    loop_tile new_tile;
    new_tile.from_x = x;
    new_tile.from_y = y;
    new_tile.distance = distance + 1;
    switch (coord(x, y)) {
        case '|': if (from_x == x && from_y == y - 1) {
                new_tile.x = x;
                new_tile.y = y+1;
            } else {
                new_tile.x = x;
                new_tile.y = y-1;
            }
            break;
        case '-': if (from_y == y && from_x == x - 1) {
                new_tile.x = x+1;
                new_tile.y = y;
            } else {
                new_tile.x = x-1;
                new_tile.y = y;
            }
            break;
        case '7': if (from_x==x-1 && from_y==y) {
                new_tile.x = x;
                new_tile.y = y+1;
            } else {
                new_tile.x = x-1;
                new_tile.y = y;
            }
            break;
        case 'F': if (from_x==x+1 && from_y==y) {
                new_tile.x = x;
                new_tile.y = y+1;
            } else {
                new_tile.x = x+1;
                new_tile.y = y;
            }
            break;
        case 'L': if (from_x==x+1 && from_y==y) {
                new_tile.x = x;
                new_tile.y = y-1;
            } else {
                new_tile.x = x+1;
                new_tile.y = y;
            }
            break;
        case 'J': if (from_x==x-1 && from_y==y) {
                new_tile.x = x;
                new_tile.y = y-1;
            } else {
                new_tile.x = x-1;
                new_tile.y = y;
            }
            break;
        default:
            std::cerr << x << "," << y << " contains " << coord(x,y) << "distance" << distance<<"\n";
 assert (false);
    }
    return new_tile;
}

void turn(std::vector<loop_tile>& q) {
    q.push_back((q.end()-1)->next_tile());
    if (coord(q.back().x, q.back().y) == 'S') return;
}

std::vector<loop_tile> part1() {
    auto start = find_start();
    int x = start.first;
    int y = start.second;
    std::cout << "Start: " << x << ", " << y << "\n";
    std::vector<loop_tile> q{};
    loop_tile tile;
    tile = loop_tile{x-1, y, x, y, 1};
    if (tile.legal_start()) {
        q.push_back(tile);
    } else {
        tile = loop_tile{x+1, y, x, y, 1};
        if (tile.legal_start()) {
            q.push_back(tile);
        } else {
            tile = loop_tile{x, y-1, x, y, 1};
            if (tile.legal_start()) {
                q.push_back(tile);
            } else {
                tile = loop_tile{x, y+1, x, y, 1};
                if (tile.legal_start()) q.push_back(tile);
            }
        }
    }

    assert (q.size() == 1);
    while (coord(q.back().x, q.back().y) != 'S') {
        turn(q);
    }

    return q;
}

u64 point (int x, int y) {
    return (u64(x) << 32) + u64(y);
}

bool in_grid (u64 p) {
    u32 x = p >> 32;
    u32 y = p;
    return !((x % 2) || (y % 2));
}

bool in_bounds (u64 p) {
    i32 x = u32(p >> 32);
    i32 y = u32(p);
    return ( (y >= -1) && (y <= 2*int(input.size()) - 1) &&
(x >= -1) && (x <= 2*int(input.size()) - 1));
}

int part2(std::vector<loop_tile>& loop_v) {
    std::unordered_set<u64> loop {};
    for (auto &t : loop_v) loop.insert(point(2*t.x, 2*t.y));

    std::cout << "Size(loop) = " << loop.size() << "\n";

    std::queue<u64> check {};
    for (int y = -1; y <= 2*int(input.size()) - 1; ++y) {
        check.push(point(-1, y));
        check.push(point(2*input[0].length() - 1, y));
    }
    for (int x = 0; x < 2*int(input[0].length()) - 1; ++x) {
        check.push(point(x, -1));
        check.push(point(x, 2*input.size()-1));
    }
    int width = 2*input[0].length() + 1;
    int height = 2*input.size() + 1;
    int area = width * height;
    int circ = (width + height -2) * 2;
    std::cout << "w: " << width << "\n";
    std::cout << "h: " << height << "\n";
    std::cout << "a: " << area << "\n";
    std::cout << "c: " << circ << "\n";
    std::cout << "Size(check) = " << check.size() << "\n";

    auto in_loop = [&](u64 p) {
        if (in_grid(p)) {
            return bool(loop.count(p));
        }
        i32 x = u32(p >> 32);
        i32 y = u32(p);
        if (loop.count(point(x-1,y)) && loop.count(point(x+1,y))) {
            char left = coord((x-1)/2, y/2);
            char right = coord((x+1)/2, y/2);
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
        if (loop.count(point(x,y-1)) && loop.count(point(x,y+1))) {
            char up = coord(x/2, (y-1)/2);
            char down = coord(x/2, (y+1)/2);
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
        u64 tbc = check.front();
        check.pop();
        if (in_bounds(tbc) && (!checked.count(tbc)) && (!in_loop(tbc))) {
            checked.insert(tbc);
            i32 x = u32(tbc >> 32);
            i32 y = u32(tbc);
            check.push(point(x+1, y));
            check.push(point(x, y+1));
            check.push(point(x-1, y));
            check.push(point(x, y-1));
        }
    }

    u64 sum = 0;
    for (auto u : checked) {
        sum += in_grid(u);
    }
    std::cout << "In grid: " << sum << "\n";
    return 140*140 - sum - loop.size();
}

int main () {
    auto p1 = part1();
    std::cout << "Part 1: " << p1.size() / 2 << "\n";
    auto p2 = part2(p1);
    std::cout << "Part 2: " << p2 << "\n";
    return 0;
}
