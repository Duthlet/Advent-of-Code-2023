#include <vector>
#include <string>
#include <iostream>
#include <shortinttypes.h>
#include <queue>
#include <sstream>

extern std::vector<std::pair<std::string, std::vector<u32>>> input;

std::vector<u32> desc(const std::string& str) {
    std::vector<u32> retval {};
    bool broken = false;
    u32 streak = 0;
    for (auto c : str) {
        if (c == '?') {
            broken = true;
            break;
        }
        if (broken) {
            if (c == '#') ++streak;
            else {
                retval.push_back(streak);
                broken = false;
                streak = 0;
            }
        } else {
            if (c == '#') {
                broken = true;
                ++streak;
            }
        }
    }
    if (broken) retval.push_back(streak);
    return retval;
}

bool matches(const std::string& str, const std::vector<u32>& info) {
    return info == desc(str);
}

bool might_match(const std::string& str, const std::vector<u32>& info) {
    auto v = desc(str);
    if (v.size() > info.size() + 1) {
        //std::cout << "v.size() = " << v.size() << "\n";
        return false;
    }
    for (u32 i = 0; i + 1 < v.size(); ++i) {
        if (v[i] != info[i]) {
            //std::cout << "v.[" << i << "] = " << v[i]  << "\n";
            return false;
        }
    }
    return true;
}

u64 possibilities(std::string& str, std::vector<u32>& info) {
    std::queue<std::string> pos {};
    pos.push(str);
    while (!pos.empty()) {
        auto front = pos.front();
        auto f = front.find('?');
        if (f == std::string::npos) break;
        pos.pop();
        /*
        pos.push(front.replace(f, 1, 1, '.'));
        pos.push(front.replace(f, 1, 1, '#'));
        */
        /*
        std::string cand = front;
        cand.replace(f, 1, 1, '.');
//        bool b =might_match(cand, info);
        if (true)       pos.push(cand);
        cand = front;
        cand.replace(f, 1, 1, '#');
//        b =might_match(cand, info);
        if (true)       pos.push(cand);
        */
        std::string cand = front;
        cand.replace(f, 1, 1, '.');
        if (might_match(cand, info)) pos.push(cand);
        /*
        else {
            std::cout << "Discarding ";
    std::cout << str << " to match ";
    for (auto i : info) std::cout << i << ", ";
    std::cout << "\n";
        }
        */
        cand = front;
        cand.replace(f, 1, 1, '#');
        if (might_match(cand, info)) pos.push(cand);
    }
    u64 sum = 0;
    while (!pos.empty()) {
        sum += matches(pos.front(), info);
        pos.pop();
    }
    /*
    std::cout << "Found " << sum << " possibilities for " << str << " to match ";
    for (auto i : info) std::cout << i << ", ";
    std::cout << "\n";
    */
    return sum;
}


u64 part1() {
    u64 sum = 0;
    for (auto &p : input) {
        sum += possibilities(p.first, p.second);
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
