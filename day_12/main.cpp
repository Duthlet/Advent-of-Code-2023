#include <vector>
#include <string>
#include <iostream>
#include <shortinttypes.h>
#include <cassert>
#include <sstream>

extern std::vector<std::pair<std::string, std::vector<u32>>> input;

bool can_match (std::string_view &str, u32 match) {
    if (str.length() < match) return false;
    for (u32 i = 0; i < match; ++i) {
        if (str[i] == '.') return false;
    }
    return (str.length() == match) || str[match] != '#';
}

std::vector<std::string_view> _get_pos (std::string_view str, u32 match) {
    std::vector<std::string_view> retval {};
    for (auto it = str.begin(); it != str.end(); ++it) {
        if (*it == '.') continue;
        auto cand = std::string_view{it, str.end()};
        if (can_match(cand, match)) {
            if (cand.length() == match) {
                retval.push_back({});
                break;
            }
            retval.push_back({it+match+1, str.end()});
        }
        if (*it == '#') break;
    }
    return retval;
}

u64 possibilities(std::string_view str, std::vector<u32>& info, const u32 i, const u32 min) {

    if (i >= info.size()) {
        for (auto it = str.begin(); it != str.end(); ++it) {
            if (*it == '#') return 0;
        }
        return 1;
    }
    u64 sum = 0;
    auto matches = _get_pos(str, info[i]);
    for (auto &m : matches) {
        sum += possibilities(m, info, i+1, min-info[i]-1);
    }
    /*
    std::cout << i << ": "<< str << " ";
    for (auto j = i; j < info.size(); ++j) std::cout << info[j] << " ";
    std::cout << "(min " << min << "): " << sum << " possibilities found!\n";
    */
    return sum;
}

u32 min (std::vector<u32>& info) {
    u32 sum = info.size() - 1;
    for (auto i : info) sum += i;
    return sum;
}

u64 part1() {
    u64 sum = 0;
    for (auto &p : input) {
        sum += possibilities({p.first.c_str()}, p.second, 0, min(p.second));
    }
    return sum;
}

int part2() {
    for (auto &p : input) {
        std::stringstream strm {};
        strm
        << p.first << '?'
        << p.first << '?'
        << p.first << '?'
        << p.first << '?'
        << p.first;
        p.first = strm.str();
        std::vector<u32> info {};
        for (int i = 0; i < 5; ++i) {
            for (auto val : p.second)
                info.push_back(val);
        }
        p.second = info;
    }
    return part1();
}

int main () {
    /*
    input = {
{"???.###",{1,1,3}},
{".??..??...?##.",{1,1,3}},
{"?#?#?#?#?#?#?#?",{1,3,1,6}},
{"????.#...#...",{4,1,1}},
{"????.######..#####.",{1,6,5}},
{"?###????????",{3,2,1}}};
*/
    auto p1 = part1();
    std::cout << "Part 1: " << p1 << "\n";
    auto p2 = part2();
    std::cout << "Part 2: " << p2 << "\n";
    return 0;
}
