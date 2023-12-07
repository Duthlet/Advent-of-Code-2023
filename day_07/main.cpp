#include <vector>
#include <iostream>
#include <string>
#include <map>
#include <algorithm>
#include <shortinttypes.h>

struct Hand {

    std::string uoh;
    u32 bid;

    int type;

    void calc_type(bool part2) {
        std::map<char, int> m {};
        for (auto c : uoh) {
            if (m.find(c) == m.end()) {
                m[c] = 1;
            } else {
                m[c] += 1;
            }
        }

        int nuj = 0;
        if (part2) {
            if (m.find('J') != m.end()) {
                nuj = m['J'];
                m['J'] = 0;
            }
        }

        std::vector<int> v;
        for (const auto& p : m) {
            if (p.second) v.push_back(p.second);
        }
        std::sort (v.begin(), v.end());

        if (part2) {
            if (v.size() != 0) {
                *(v.end()-1) += nuj;
            } else {
                v.push_back(nuj);
            }
        }

        if (v == std::vector{1, 1, 1, 1, 1})
            type = 0;
        else if (v == std::vector{1, 1, 1, 2})
            type = 1;
        else if (v == std::vector{1, 2, 2})
            type = 2;
        else if (v == std::vector{1, 1, 3})
            type = 3;
        else if (v == std::vector{2, 3})
            type = 4;
        else if (v == std::vector{1, 4})
            type = 5;
        else if (v == std::vector{5})
            type = 6;
        else {
            throw(1);
        }
    }

    Hand (std::string s, u32 i) : uoh(s), bid(i) {
        calc_type(false);
    }


};


int cmpcard (char a, char b, std::vector<char> &card_values) {
    auto it_a = std::find(card_values.begin(), card_values.end(), a);
    auto it_b = std::find(card_values.begin(), card_values.end(), b);
    if (it_a < it_b) return 1;
    if (it_a > it_b) return -1;
    return 0;
}

bool cmp (const Hand& a, const Hand& b, std::vector<char> &card_values) {
    if(a.type > b.type) return true;
    if(a.type < b.type) return false;
    for(int i = 0; i < 5; ++i) {
        auto cc = cmpcard(a.uoh[i], b.uoh[i], card_values);
        if (cc == 1) return true;
        if (cc == -1) return false;
    }
    throw(2);
}

u64 sum (const std::vector<Hand> &hands) {
    u64 sum = 0;
    unsigned int i = 1;
    for (auto it = hands.rbegin(); it != hands.rend(); ++it) {
        sum += i * it->bid;
        ++i;
    }
    return sum;
}

#include "input.inc"

int part1() {
    std::vector<char> card_values
    {'A','K','Q','J','T','9','8','7','6','5','4','3','2'};
    std::sort(input.begin(), input.end(), [&](const Hand& a, const Hand& b){return cmp(a,b,card_values);});
    return sum(input);
}

int part2() {
    for (auto &h : input) h.calc_type(true);
    std::vector<char> card_values
    {'A','K','Q','T','9','8','7','6','5','4','3','2','J'};
    std::sort(input.begin(), input.end(), [&](const Hand& a, const Hand& b){return cmp(a,b,card_values);});
    return sum(input);
}

int main () {
    auto p1 = part1();
    auto p2 = part2();
    std::cout << "Part 1: " << p1 << "\n";
    std::cout << "Part 2: " << p2 << "\n";
    return 0;
}
