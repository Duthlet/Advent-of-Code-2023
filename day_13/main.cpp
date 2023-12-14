#include <vector>
#include <iostream>
#include <shortinttypes.h>
#include <cassert>
#include <string>
#include "input.inc"

std::vector<std::string> transpose (const std::vector<std::string>& in) {
    std::vector<std::string> out (in[0].length(), std::string(in.size(), ' '));
    assert (out.size() == in[0].length());
    for (u32 i = 0; i < in.size(); ++i) {
        assert (in[i].length() == in[0].length());
        for (u32 j = 0; j < in[0].length(); ++j) {
            assert (out[j].length() == out[0].length());
            out[j][i] = in[i][j];
            }
    }
    return out;
}

u32 hor_sym (const std::vector<std::string>& in, u32 exclude) {

    auto check = [&](u32 i) {
        for (u32 j = 1; (i+j < in.size()) && i > j; ++j)
            if (in[i-1-j] != in[i+j]) return false;
        return true;
    };

    for (u32 i = 1; i < in.size(); ++i) {
        if (i == exclude) continue;
        if (in[i] == in[i-1])
            if (check(i)) return i;
    }
    return 0;
}

void print (const std::vector<std::string>& in) {
    for (auto &r : in) std::cout << r << "\n";
}

u32 sym (const std::vector<std::string>& in, u32 exclude) {
    u32 i = hor_sym(in, exclude % 100 ? 0 : exclude / 100);
    if (i) {
        /*
        std::cout << "Found symmetry for " << 100*i << " in\n";
        print(in);
        */
        return 100 * i;
    }
    i = hor_sym(transpose(in), exclude % 100 ? exclude % 100 : 0);
    /*
    std::cout << "Found symmetry for " << i << " in\n";
    print(in);
    */
    return i;
}

u64 part1() {
    u64 sum = 0;
    for (const auto &pat : input) {
        sum += sym(pat, 0);
    }
    return sum;
}

void flip (char &c) {
    c = (c == '.' ? '#' : '.');
}

u32 mod_sym (std::vector<std::string>& in) {
    u32 exclude = sym(in, 0);
    for (auto &r : in)
        for (auto &c : r) {
            flip(c);
            u32 i = sym(in, exclude);
            if (i) return i;
            flip(c);
        }
    assert(false);
}

int part2() {
    u64 sum = 0;
    for (auto &pat : input) {
        sum += mod_sym(pat);
    }
    return sum;
}

int main () {
/*
input = {{
"#.##..##.",
"..#.##.#.",
"##......#",
"##......#",
"..#.##.#.",
"..##..##.",
"#.#.##.#."
},{
"#...##..#",
"#....#..#",
"..##..###",
"#####.##.",
"#####.##.",
"..##..###",
"#....#..#"}};
*/
    auto p1 = part1();
    std::cout << "Part 1: " << p1 << "\n";
    auto p2 = part2();
    std::cout << "Part 2: " << p2 << "\n";
    return 0;
}
