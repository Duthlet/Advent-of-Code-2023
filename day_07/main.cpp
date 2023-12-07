#include <vector>
#include <iostream>
#include <string>
#include <map>
#include <algorithm>
#include <shortinttypes.h>

struct Hand {
    std::string hand;
    u32 bid;
    int type;

    Hand (std::string s, u32 i) : hand(s), bid(i) {}

    void calc_type(bool part2) {

        std::map<char, int> m {};
        for (auto c : hand) ++m[c];

        int num_jokers = 0;
        if (part2) {
            num_jokers = m['J'];
            m.erase('J');
        }

        std::vector<int> v;
        for (const auto& p : m) {
            if (p.second) v.push_back(p.second);
        }
        std::sort (v.begin(), v.end());

        if (part2) {
            if (v.empty()) v.push_back(0);
            v.back() += num_jokers;
        }

        type =
            (std::map<std::vector<int>, int> {
            {{1, 1, 1, 1, 1}, 0},
            {{1, 1, 1, 2}, 1},
            {{1, 2, 2}, 2},
            {{1, 1, 3}, 3},
            {{2, 3}, 4},
            {{1, 4}, 5},
            {{5}, 6},
            }).at(v);
    }
};

int cmpcard (char a, char b, const std::vector<char> &card_values) {
    auto it_a = std::find(card_values.begin(), card_values.end(), a);
    auto it_b = std::find(card_values.begin(), card_values.end(), b);
    if (it_a < it_b) return -1;
    if (it_a > it_b) return 1;
    return 0;
}

bool cmp (const Hand& a, const Hand& b, const std::vector<char> &card_values) {
    if(a.type < b.type) return true;
    if(a.type > b.type) return false;
    for(int i = 0; i < 5; ++i) {
        switch (cmpcard(a.hand[i], b.hand[i], card_values)) {
            case 1 : return true;
            case -1: return false;
        }
    }
    return false;
}

u64 sum (const std::vector<Hand> &hands) {
    u64 sum = 0;
    unsigned int rank = 1;
    for (const auto& hand : hands) {
        sum += rank * hand.bid;
        ++rank;
    }
    return sum;
}

#include "input.inc"

void sort_input(const std::vector<char> &card_values) {
    std::sort(
        input.begin(),
        input.end(),
        [&](const Hand& a, const Hand& b){return cmp(a,b,card_values);}
    );
}

int part1() {
    for (auto &h : input) h.calc_type(false);
    sort_input({'A','K','Q','J','T','9','8','7','6','5','4','3','2'});
    return sum(input);
}

int part2() {
    for (auto &h : input) h.calc_type(true);
    sort_input({'A','K','Q','T','9','8','7','6','5','4','3','2','J'});
    return sum(input);
}

int main () {
    auto p1 = part1();
    auto p2 = part2();
    std::cout << "Part 1: " << p1 << "\n";
    std::cout << "Part 2: " << p2 << "\n";
    return 0;
}
