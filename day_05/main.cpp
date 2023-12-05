#include <vector>
#include <iostream>
#include <cstdint>
#include <limits>

class mapping {
    uint64_t dest;
    uint64_t src;
    uint64_t len;

    public:

    uint64_t map(uint64_t &x) const {
        if (x >= src && x < src + len) {
            uint64_t y = src + len - x;
            x = (dest + x - src);
            return y;
        }
        return 0;
    }

    mapping (uint64_t d, uint64_t s, uint64_t l) : dest(d), src(s), len(l) {}
};

#include "input.inc"

uint64_t map (uint64_t& x, const std::vector<mapping> &mapp) {
    uint64_t y;
    for (const auto& m : mapp) {
        if (auto y = m.map(x)) {
            return y;
        }
    }
    return std::numeric_limits<uint64_t>::max();
}

uint64_t map1 (uint64_t& x) {
    uint64_t y = std::numeric_limits<uint64_t>::max();
    y = std::min(y, map (x, seed_to_soil));
    y = std::min(y, map (x, soil_to_fertilizer));
    y = std::min(y, map (x, fertilizer_to_water));
    y = std::min(y, map (x, water_to_light));
    y = std::min(y, map (x, light_to_temperature));
    y = std::min(y, map (x, temperature_to_humidity));
    y = std::min(y, map (x, humidity_to_location));
    return y == std::numeric_limits<uint64_t>::max() ? 0 : y;
}

uint64_t part1() {
    uint64_t min = std::numeric_limits<uint64_t>::max();
    for (auto s : seeds) {
        map1(s);
        if (s < min)
            min = s;
    }
    return min;
}

uint64_t map2 (uint64_t start, uint64_t len) {
    uint64_t min = std::numeric_limits<uint64_t>::max();
    for (uint64_t i = start; i < start + len; ++i) {
        auto cand = i;
        auto y = map1(cand);
        if (cand < min)
            min = cand;
        if (y) i+= y-1;
    }
    return min;
}

int part2() {
    uint64_t min = std::numeric_limits<uint64_t>::max();
    for (auto it = seeds.begin(); it != seeds.end(); ++it) {
        auto start = *it;
        ++it;
        auto len = *it;
        auto cand = map2(start, len);
        if (cand < min)
            min = cand;
    }
    return min;
}

int main () {
    auto p1 = part1();
    auto p2 = part2();
    std::cout << "Part 1: " << p1 << "\n";
    std::cout << "Part 2: " << p2 << "\n";
    return 0;
}
