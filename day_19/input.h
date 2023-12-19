#pragma once

#include <vector>
#include <shortinttypes.h>
#include <unordered_map>
#include <string>

struct item {
    u32 x;
    u32 m;
    u32 a;
    u32 s;
};

extern std::unordered_map<std::string, std::vector<std::string>> input;
extern std::vector<item> items;
