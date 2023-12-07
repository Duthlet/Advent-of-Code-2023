#include <vector>
#include <iostream>
#include <shortinttypes.h>

class mapping {
    u64 dest;
    u64 src;
    u64 len;

    public:

    u64 map(u64 &x) const {
        if (x >= src && x < src + len) {
            u64 y = src + len - x;
            x = (dest + x - src);
            return y;
        }
        return 0;
    }

    mapping (u64 d, u64 s, u64 l) : dest(d), src(s), len(l) {}
};

#include "input.inc"

u64 map (u64& x, const std::vector<mapping> &mapp) {
    u64 y;
    for (const auto& m : mapp) {
        if (auto y = m.map(x)) {
            return y;
        }
    }
    return U64MAX;
}

u64 map1 (u64& x) {
    u64 y = U64MAX;
    y = std::min(y, map (x, seed_to_soil));
    y = std::min(y, map (x, soil_to_fertilizer));
    y = std::min(y, map (x, fertilizer_to_water));
    y = std::min(y, map (x, water_to_light));
    y = std::min(y, map (x, light_to_temperature));
    y = std::min(y, map (x, temperature_to_humidity));
    y = std::min(y, map (x, humidity_to_location));
    return y == U64MAX ? 0 : y;
}

u64 part1() {
    u64 min = U64MAX;
    for (auto s : seeds) {
        map1(s);
        if (s < min)
            min = s;
    }
    return min;
}

u64 map2 (u64 start, u64 len) {
    u64 min = U64MAX;
    for (u64 i = start; i < start + len; ++i) {
        auto cand = i;
        auto y = map1(cand);
        if (cand < min)
            min = cand;
        if (y) i+= y-1;
    }
    return min;
}

int part2() {
    u64 min = U64MAX;
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
