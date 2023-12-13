#include <vector>
#include <string>
#include <iostream>
#include <shortinttypes.h>
#include <cassert>
#include <sstream>
#include <map>
#include <unordered_map>

extern std::vector<std::pair<std::string, std::vector<u32>>> input;

bool can_match (std::string_view str, u32 match) {
    if (str.length() < match) return false;
    for (u32 i = 0; i < match; ++i) {
        if (str[i] == '.') return false;
    }
    return (str.length() == match) || str[match] != '#';
}

std::vector<u32> _get_pos (const std::string& str, u32 match) {
    std::vector<u32> retval {};
    bool skip = false;
    for (auto it = str.begin(); it != str.end() + 1 - match; ++it) {
        if (skip) {
            if (*it != '#') skip = false;
            continue;
        }
        if (*it == '.') continue;
        if (can_match({it, str.end()}, match)) {
            retval.push_back(it - str.begin());
        }
        if (*it == '#') skip = true;
    }
    return retval;
}

std::map <u32, std::vector<u32>> pos_by_match (const std::string& str, const std::vector<u32>& info) {
    std::map <u32, std::vector<u32>> retval {};
    retval[0] = {};
    for (u32 i = 0; i < str.size(); ++i)
        if (str[i] == '#') retval[0].push_back(i);
    for (auto i : info) {
        if (retval.find(i) == retval.end())
            retval[i] = _get_pos(str, i);
    }
    return retval;
}

std::unordered_map<u64, u64> cache {};


u64 possibilities(std::map<u32, std::vector<u32>> &pbm, u32 os, std::vector<u32>& info, const u32 i, const u32 min, const u32 size) {

    u64 cache_index =(u64(os) << 32) | u64(i);

    auto cached = cache.find(cache_index);
    if (cached != cache.end()) {
        return cached->second;
    }

    if (i >= info.size()) return pbm[0].empty() || pbm[0].back() < os;

    if (os + min > size) return 0;

    u64 sum = 0;
    auto matches = pbm[info[i]];
    u32 next_broken = U32MAX;
    for (auto i : pbm[0]) {
        if (i < os) continue;
        next_broken = i;
        break;
    }
    for (auto &m : matches) {
        if (m < os) continue;
        if (m > next_broken) break;
        sum += possibilities(pbm, m + info[i] + 1, info, i+1, min-info[i]-1, size);
    }
    cache[cache_index] = sum;
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
        auto matches = pos_by_match({p.first.c_str()}, p.second);
        /*
        std::cout << "In " << p.first << ":\n";
        for (auto &p : matches) {
            std::cout << p.first << ": ";
            for (auto i : p.second) std::cout << i << ", ";
            std::cout << "\n";
        }
        */
        cache = std::unordered_map<u64, u64> {};
        auto pos = possibilities(matches, 0, p.second, 0, min(p.second), p.first.length());
 //       std::cout << "In " << p.first << ": " << pos << "\n";
        sum += pos;
    }
    return sum;
}

u64 part2() {
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
