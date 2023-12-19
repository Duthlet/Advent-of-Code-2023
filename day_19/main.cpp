#include <vector>
#include <iostream>
#include <shortinttypes.h>
#include <unordered_map>
#include <string>
#include <cassert>

#include "input.h"

u32 fail() {
    assert(false);
}

bool accept (const item &i) {
/*
    std::cout << "Item "
    << i.x << ","
    << i.m << ","
    << i.a << ","
    << i.s << "\n";
    */
    std::string key = "in";
    std::string next;
    while(1) {
//        std::cout << key << " ->\n";
        for (const std::string& action : input[key]) {
            auto f = action.find(':');
            if (f == std::string::npos) {
                next = action;
                if (next == "A") return true;
                if (next == "R") return false;
                key = next;
                break;
            }
            std::string condition = action.substr(0, f);
            u32 val =
                condition[0] == 'x' ? i.x :
                condition[0] == 'm' ? i.m :
                condition[0] == 'a' ? i.a :
                condition[0] == 's' ? i.s : fail();
            u32 cmp = stoul(condition.substr(2, std::string::npos));
            bool todo = false;
            if (condition[1] == '<') todo = val < u32(cmp);
            else if (condition[1] == '>') todo = val > u32(cmp);
            else assert(false);
            if (!todo) continue;
            next = action.substr(f+1, std::string::npos);
            if (next == "A") return true;
            if (next == "R") return false;
            key = next;
            break;
        }
    }
    assert(false);
}

u64 span (const item &min, const item &max) {
/*
    std::cout << "Min "
    << min.x << ","
    << min.m << ","
    << min.a << ","
    << min.s << "\n";
    std::cout << "Max "
    << max.x << ","
    << max.m << ","
    << max.a << ","
    << max.s << "\n";*/
    return
        u64(max.x - min.x + 1) *
        u64(max.m - min.m + 1) *
        u64(max.a - min.a + 1) *
        u64(max.s - min.s + 1);
}

u64 possibilities (item min, item max) {
/*
    std::cout << "Min "
    << min.x << ","
    << min.m << ","
    << min.a << ","
    << min.s << "\n";
    std::cout << "Max "
    << max.x << ","
    << max.m << ","
    << max.a << ","
    << max.s << "\n";
*/
    std::string key = "in";
    std::string next;
    while(1) {
//        std::cout << key << " ->\n";
        for (const std::string& action : input[key]) {
            auto f = action.find(':');
            if (f == std::string::npos) {
                next = action;
                goto act;
            }
            {
            std::string condition = action.substr(0, f);
            u32& val_min =
                condition[0] == 'x' ? min.x :
                condition[0] == 'm' ? min.m :
                condition[0] == 'a' ? min.a :
                condition[0] == 's' ? min.s : min.s;
            u32& val_max =
                condition[0] == 'x' ? max.x :
                condition[0] == 'm' ? max.m :
                condition[0] == 'a' ? max.a :
                condition[0] == 's' ? max.s : min.s;
            u32 cmp = stoul(condition.substr(2, std::string::npos));
            next = action.substr(f+1, std::string::npos);
            if (condition[1] == '<') {
                if (val_min >= cmp) {
                    continue;
                }
                if (val_max < cmp) {
                    goto act;
                }

                u32 old_min = val_min;
                u32 old_max = val_max;

                val_max = cmp - 1;
                item new_max = max;
                val_max = old_max;

                val_min = cmp;
                item new_min = min;
                val_min = old_min;

                return possibilities(min, new_max)
                    + possibilities(new_min, max);
            }
            else if (condition[1] == '>') {
                if (val_min > cmp) {
                    goto act;
                }
                if (val_max <= cmp) {
                    continue;
                }

                u32 old_min = val_min;
                u32 old_max = val_max;

                val_max = cmp;
                item new_max = max;
                val_max = old_max;

                val_min = cmp + 1;
                item new_min = min;
                val_min = old_min;

                return possibilities(min, new_max)
                    + possibilities(new_min, max);
            }
            else assert(false);
            continue;
            }
            act:
            if (next == "A") {
            u64 n = span(min, max);
                std::cout << n << " accepted\n";
                return n;
            }
            if (next == "R") {
            u64 n = span(min, max);
                std::cout << n << " rejected\n";
                return 0;
            }
            key = next;
            break;
        }
    }
    assert(false);
}

u64 part1() {
    u64 sum = 0;
    for (const auto &i : items) {
        if (accept(i)) sum += i.x + i.m + i.a + i.s;
    }
    return sum;
}

u64 part2() {
    return possibilities(
    {1,1,1,1},
    {4000,4000,4000,4000}
    );
}

int main () {
/*
input = {
{"px",{"a<2006:qkq","m>2090:A","rfg"}},
{"pv",{"a>1716:R","A"}},
{"lnx",{"m>1548:A","A"}},
{"rfg",{"s<537:gd","x>2440:R","A"}},
{"qs",{"s>3448:A","lnx"}},
{"qkq",{"x<1416:A","crn"}},
{"crn",{"x>2662:A","R"}},
{"in",{"s<1351:px","qqz"}},
{"qqz",{"s>2770:qs","m<1801:hdj","R"}},
{"gd",{"a>3333:R","R"}},
{"hdj",{"m>838:A","pv"}}};

items = {
{787,2655,1222,2876},
{1679,44,2067,496},
{2036,264,79,2244},
{2461,1339,466,291},
{2127,1623,2188,1013}};
*/
    auto p1 = part1();
    std::cout << "Part 1: " << p1 << "\n";
    auto p2 = part2();
    std::cout << "Part 2: " << p2 << "\n";
    return 0;
}
