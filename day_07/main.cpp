#include <vector>
#include <iostream>
#include <string>
#include <map>
#include <algorithm>
#include <fstream>
#include <shortinttypes.h>

template <int part>
class Hand {
    static const std::vector<char> card_values;
    static const bool use_jokers;
    static const std::map<std::vector<int>, int> types;

    static int calc_type(const std::string& _hand) {
        std::map<char, int> m {};
        for (auto c : _hand) ++m[c];

        int num_jokers = 0;
        if (use_jokers) {
            num_jokers = m['J'];
            m.erase('J');
        }

        std::vector<int> v {};
        for (const auto& p : m) v.push_back(p.second);
        std::sort (v.begin(), v.end());

        if (use_jokers) {
            if (v.empty()) v.push_back(0);
            v.back() += num_jokers;
        }

        return types.at(v);
    }

    public:
    std::string hand;
    u32 bid;
    int type;

    Hand (std::string _hand, u32 _bid) : hand(_hand), bid(_bid), type(calc_type(_hand)) {}

    auto operator<=>(const Hand& other) const {
        auto comparison = type <=> other.type;
        if (comparison != 0) return comparison;
        for(int i = 0; i < 5; ++i) {
            comparison = std::find(card_values.begin(), card_values.end(), hand[i])
                         <=> std::find(card_values.begin(), card_values.end(), other.hand[i]);
            if (comparison != 0) return comparison;
        }
        return comparison;
    }
};

template <int part> const std::map<std::vector<int>, int> Hand<part>::types {
    {{1, 1, 1, 1, 1}, 0},
    {{1, 1, 1, 2}, 1},
    {{1, 2, 2}, 2},
    {{1, 1, 3}, 3},
    {{2, 3}, 4},
    {{1, 4}, 5},
    {{5}, 6},
};
template <> const bool Hand<1>::use_jokers(false);
template <> const std::vector<char> Hand<1>::card_values {'2','3','4','5','6','7','8','9','T','J','Q','K','A'};
template <> const bool Hand<2>::use_jokers(true);
template <> const std::vector<char> Hand<2>::card_values {'J','2','3','4','5','6','7','8','9','T','Q','K','A'};

template <int part>
u64 sum (const std::vector<Hand<part>> &hands) {
    u64 sum = 0;
    unsigned int rank = 1;
    for (const auto& hand : hands) {
        sum += rank * hand.bid;
        ++rank;
    }
    return sum;
}

int part1(std::vector<std::string> &input) {
    std::vector<Hand<1>> v {};
    for (auto &line : input) v.emplace_back(line.substr(0, 5), stoi(line.substr(6, std::string::npos)));
    std::sort(v.begin(), v.end());
    return sum(v);
}

int part2(std::vector<std::string> &input) {
    std::vector<Hand<2>> v {};
    for (auto &line : input) v.emplace_back(line.substr(0, 5), stoi(line.substr(6, std::string::npos)));
    std::sort(v.begin(), v.end());
    return sum(v);
}

int main () {
    std::vector<std::string> input;
    std::ifstream file("input");
    while (1) {
        std::string line;
        std::getline(file, line);
        if (file.eof()) break;
        input.push_back(line);
    }
    auto p1 = part1(input);
    auto p2 = part2(input);
    std::cout << "Part 1: " << p1 << "\n";
    std::cout << "Part 2: " << p2 << "\n";
    return 0;
}
