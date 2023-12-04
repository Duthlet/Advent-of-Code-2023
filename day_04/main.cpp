#include <vector>
#include <utility>
#include <iostream>
#include <algorithm>
#include <cassert>
#include <cstdint>

#include "input.inc"

int wns(std::vector<int> wn, std::vector<int> on) {
    std::vector<int> in {};
    std::sort(wn.begin(), wn.end());
    std::sort(on.begin(), on.end());
    std::set_intersection(wn.begin(),wn.end(),
                          on.begin(),on.end(),
                          back_inserter(in));
    return in.size();
}

// winning numbers first
int part1() {
    int sum = 0;
    for (const auto& line : input) {
        int n =wns(line.first, line.second);
        if (n) sum += 1 << (n - 1);
    }
    return sum;
}

int part2() {
    std::vector<uint64_t> cards;
    for (auto &x : input) {
        cards.push_back(1);
    }
    uint64_t sum;
    for (int i = 0; i < input.size(); ++i) {
        int n =wns(input[i].first, input[i].second);
        sum += cards[i];
        for (int j = 1; j <= n; ++j) {
            assert(i+j < cards.size());
            cards[i+j] += cards[i];
        }
    }
    return sum;
}

int main () {
    auto p1 = part1();
    auto p2 = part2();
    std::cout << "Part 1: " << p1 << "\n";
    std::cout << "Part 2: " << p2 << "\n";
    return 0;
}
