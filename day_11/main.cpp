#include <vector>
#include <string>
#include <iostream>
#include <shortinttypes.h>
#include <point.h>

#include "input.inc"
/*
std::vector<std::string> input {
"...#......",
".......#..",
"#.........",
"..........",
"......#...",
".#........",
".........#",
"..........",
".......#..",
"#...#....."};
*/
void inflate_y () {
    for (u32 i = 0; i < input.size(); ++i) {
        if (input[i].find("#") == std::string::npos) {
            input[i] = std::string(input[i].size(), 'y');
        }
    }
}

bool empty_row( u32 i ) {
    for (u32 j = 0; j < input.size(); ++j) {
        if (input[j][i] == '#') return false;
    }
    return true;
}

void inflate_x () {
    for (u32 i = 0; i < input[0].length(); ++i) {
        if (empty_row(i)) {
            for (u32 j = 0; j < input.size(); ++j) {
                input[j][i] = 'x';
            }
        }
    }
}

//constexpr i32 inflation = 1;
constexpr i32 inflation = 999999;

u64 d(const point& lhs, const point& rhs) {
    return abs(lhs.x - rhs.x) + abs(lhs.y - rhs.y);
}

u64 d(const std::pair<point, point>& lhs,const std::pair<point, point>& rhs) {
    return d(lhs.first, rhs.first) + d(lhs.second, rhs.second) * inflation;
}


u64 part1() {
    inflate_y();
    inflate_x();
    std::vector<std::pair<point, point>> galaxies {};
    i32 y_inf = 0;
    i32 x_inf;
    for (u32 y = 0 ; y < input.size(); ++y) {
        if (input[y][0] == 'y') ++y_inf;
        x_inf = 0;
        for (u32 x = 0 ; x < input[0].length(); ++x) {
            if (input[y][x] == 'x') ++x_inf;
            if (input[y][x] == '#') {
                galaxies.push_back({{i32(x), i32(y)}, {x_inf, y_inf}});
            }
        }
    }

    std::cout << "Found " << galaxies.size() << " galaxies\n";

    u64 sum = 0;
    for (u32 i = 0; i < galaxies.size(); ++i)
        for (u32 j = i+1; j < galaxies.size(); ++j)
            sum += d(galaxies[i], galaxies[j]);

    return sum;
}

int part2() {
    return 0;
}

int main () {
    auto p1 = part1();
    std::cout << "Part 1: " << p1 << "\n";
    auto p2 = part2();
    std::cout << "Part 2: " << p2 << "\n";
    return 0;
}
