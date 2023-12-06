#include <vector>
#include <iostream>
#include <shortinttypes.h>

#include "input.inc"

u64 wins (u64 t, u64 d) {
    u64 w = 0;
    for (u64 i = 1; i < t; ++i)
        w += (t-i)*i > d;
    return w;
}

int part1() {
    u64 prod = 1;
    for (int i = 0; i < Time.size() - 1; ++i)
        prod *= wins(Time[i], Distance[i]);
    return prod;
}

int part2() {
    return wins(Time.back(), Distance.back());
}

int main () {
    auto p1 = part1();
    auto p2 = part2();
    std::cout << "Part 1: " << p1 << "\n";
    std::cout << "Part 2: " << p2 << "\n";
    return 0;
}
