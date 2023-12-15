#include <vector>
#include <iostream>
#include <shortinttypes.h>
#include <string>
#include <cassert>

#include "input.inc"

u8 hash (const std::string& line) {
    u8 retval = 0;
    for (u8 b : line) {
        retval += b;
        retval *= 17;
    }
    return retval;
}

u64 part1() {
    u64 sum = 0;
    for (const auto& l : input)
        sum += hash(l);
    return sum;
}

struct info {
    std::string label;
    char operation;
    u32 value;
};

info split_line (const std::string& line) {
    info retval {};
    bool number = false;
    bool assertion = true;
    for (auto c : line) {
        assert(assertion);
        if (number) {
            assertion = false;
            retval.value = c - '0';
            continue;
        }
        if (c == '=' || c == '-') {
            retval.operation = c;
            number = true;
            continue;
        }
        retval.label.push_back(c);
    }
    return retval;
}

void remove (std::vector<std::pair<std::string, u32>>& box, const std::string& label) {
    for (auto it = box.begin(); it != box.end(); ++it) {
        if (it->first == label) {
            box.erase(it);
            break;
        }
    }
}

void replace (std::vector<std::pair<std::string, u32>>& box, const std::string& label, u32 val) {
    for (auto it = box.begin(); it != box.end(); ++it) {
        if (it->first == label) {
            it->second = val;
            return;
        }
    }
    box.push_back({label, val});
}

u64 part2() {
    std::vector<std::pair<std::string, u32>> boxes[256];
    for (const auto& l : input) {
        info bla = split_line (l);
        u8 h = hash(bla.label);
        if (bla.operation == '-') {
            remove(boxes[h], bla.label);
        } else {
            replace(boxes[h], bla.label, bla.value);
        }
    }
    u64 sum = 0;
    for (u32 h = 0; h < 256; ++h) {
        for (u32 i = 0; i < boxes[h].size(); ++i)
            sum += (h+1)*(i+1)*boxes[h][i].second;
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
