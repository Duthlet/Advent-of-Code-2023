#include <vector>
#include <iostream>
#include <shortinttypes.h>

#include "input.inc"

std::vector<i64> diff (const std::vector<i64>& in) {
    std::vector<i64> out {};
    i64 last = in[0];
    for (u32 i = 1; i < in.size(); ++i) {
        out.push_back(in[i] - last);
        last = in[i];
    }
    return out;
}
bool zero (const std::vector<i64>& in) {
    for (auto i : in)
        if (i) return false;
    return true;
}

i64 extrapolate (const std::vector<i64>& in, bool part2) {
    std::vector<std::vector <i64>> diffs {};
    diffs.push_back(in);
    while (!zero(diffs.back())) {
        diffs.push_back(diff(diffs.back()));
    }
    i64 extra = 0;
    for (auto it = ++diffs.rbegin(); it != diffs.rend(); ++it) {
        if (part2) extra = it->front() - extra;
        else extra += it->back();
    }
    return extra;
}


int part1() {
    i64 sum = 0;
    for (const auto& line : input) {
        sum += extrapolate(line, false);
    }
    return sum;
}

int part2() {
    i64 sum = 0;
    for (const auto& line : input) {
        sum += extrapolate(line, true);
    }
    return sum;
}

int main () {
    auto p1 = part1();
    std::cout << "Part 1: " << p1 << "\n";
    auto p2 = part2();
    std::cout << "Part 2: " << p2 << "\n";
    return 0;
}
