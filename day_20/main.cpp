#include <vector>
#include <iostream>
#include <shortinttypes.h>
#include <string>
#include <cassert>
#include <unordered_map>
#include <queue>
#include <set>
#include <algorithm>

#include "input.inc"

u64 lows = 0;
u64 highs = 0;
u32 broadcaster;
u32 goal;

struct modul;

std::vector<modul*> modules;

struct request {
    u32 sender;
    u32 receiver;
    bool high;
};

std::queue<request> requests;
bool handle_request();

struct modul {
    u32 _id;
    std::vector<u32> dests;
    bool on;

    std::vector<u32> inputs;

    void register_input(u32 id) {
        inputs.push_back(id);
    }

    virtual void low_pulse(u32 id) = 0;
    virtual void high_pulse(u32 id) = 0;

    modul (u32 id, std::vector<u32> d) : _id(id), dests(d), on(false) {}

    void register_as_input() {
        for (u32 u : dests) {
            if (u != U32MAX) modules[u]->register_input(_id);
        }
    }
    void send_high(){
        on = true;
        for (auto u : dests) requests.push({_id, u, true});
        while(handle_request());
    }
    void send_low() {
        on = false;
        for (auto u : dests) requests.push({_id, u, false});
        while(handle_request());
    }

    virtual ~modul() = default;
};

struct mockgoal : modul {
    mockgoal (u32 id, std::vector<u32> d) : modul(id, d) {}
    bool triggered;
    void low_pulse(u32 id) override { if (modules[dests[0]]->on) triggered = true; }
    void high_pulse(u32 id) override { if (modules[dests[0]]->on) triggered = true; }
};

struct broadcast : modul {
    broadcast (u32 id, std::vector<u32> d) : modul(id, d) {}
    void low_pulse(u32 id) override { send_low(); }
    void high_pulse(u32 id) override { send_high(); }
};

struct flipflop : modul {
    flipflop (u32 id, std::vector<u32> d) : modul(id, d) {}
    void low_pulse(u32 id) override { on ? send_low() : send_high(); }
    void high_pulse(u32 id) override { }
};

struct conjunction : modul {
    conjunction (u32 id, std::vector<u32> d) : modul(id, d) {}
    bool all_true () {
        for (u32 u : inputs) if (!modules[u]->on) return false;
        return true;
    }
    void low_pulse(u32 id) override { all_true() ? send_low() : send_high(); }
    void high_pulse(u32 id) override { all_true() ? send_low() : send_high(); }
};

bool handle_request () {
    if (requests.empty()) return false;
    auto& r = requests.front();
    requests.pop();
    if (r.high) {
        ++highs;
        if (r.receiver != U32MAX) modules[r.receiver]->high_pulse(r.sender);
    } else {
        ++lows;
        if (r.receiver != U32MAX) modules[r.receiver]->low_pulse(r.sender);
    }
    return true;
}

void push_button () {
    assert (requests.empty());
    ++lows;
    modules[broadcaster]->low_pulse(0);
}

void create_modules() {
    std::unordered_map<std::string, u32> name_table;
    name_table["rx"] = U32MAX;
    name_table["output"] = U32MAX;
    std::vector<u32> types;
    constexpr u32 ff = 0;
    constexpr u32 cj = 1;
    constexpr u32 bc = 2;
    for (u32 id = 0; id < _input.size(); ++id) {
        std::string name;
        u32 type;
        std::string label = _input[id].first;
        switch(label[0]) {
            case '%':
                name = label.substr(1, std::string::npos);
                type = ff;
                break;
            case '&':
                type = cj;
                name = label.substr(1, std::string::npos);
                break;
            default:
                assert (label == "broadcaster");
                type = bc;
                name = label;
        }
        types.push_back(type);
        name_table[name] = id;
    }
    for (u32 id = 0; id < _input.size(); ++id) {
        std::vector<u32> dests = {};
        for (const std::string& s : _input[id].second) {
            dests.push_back(name_table[s]);
        }
        switch (types[id]) {
            case ff:
                modules.push_back(new flipflop(id, dests));
                break;
            case cj:
                modules.push_back(new conjunction(id, dests));
                break;
            case bc:
                modules.push_back(new broadcast(id, dests));
                broadcaster = id;
                break;
            default:
                assert(false);
        }
    }
    for (const auto& ptr: modules) {
        ptr->register_as_input();
    }


}
u64 part1() {
    create_modules();
    for (i32 i = 0; i < 1000; ++i) {
        push_button();
    }
    return highs * lows;
}

std::set<u32> visited(u32 start, u32 exclude, bool backwards) {
    std::set<u32> retval;
    std::queue<u32> candidates;
    candidates.push(start);
    while (!candidates.empty()) {
        u32 cand = candidates.front();
        candidates.pop();
        if (cand == exclude) continue;
        if (cand == U32MAX) continue;
        if (!retval.contains(cand)) {
            retval.insert(cand);
            for (u32 u : (backwards ? modules[cand]->inputs : modules[cand]->dests))
                candidates.push(u);
        }
    }
    return retval;
}

std::vector<u32> complement (std::set<u32> in) {
    std::vector<u32> retval;
    for (u32 i = 0; i < modules.size(); ++i) {
        if (!in.contains(i)) retval.push_back(i);
    }
    return retval;
}

std::vector<u32> skipped(u32 start, u32 exclude, bool backwards) {
    return complement(visited(start, exclude, backwards));
}

std::vector<u32> intersection (const std::vector<u32> &lhs, const std::vector<u32> &rhs) {
    std::vector<u32> retval;
    std::set_intersection(lhs.begin(), lhs.end(), rhs.begin(), rhs.end(),
                  std::back_inserter(retval));
    return retval;
}


struct analysis {
    u32 start;
    u32 end;
    std::vector<u32> nodes;

    analysis (u32 s, u32 e, std::vector<u32> n) : start(s), end(e), nodes(n) {
        find_cycle();
    }

    u64 cycle_length;

    private:
    u64 cycle_start;
    std::vector<bool> triggers;

    void push_start () {
        assert (requests.empty());
        modules[start]->low_pulse(broadcaster);
    }

    u64 state() {
        u64 retval = 0;
        for (u32 u : nodes) {
            if (modules[u]->on) retval |= (u64(1) << u);
        }
        return retval;
    }

    void assert_everything_is_good() {
        // make sure there is only one trigger
        u64 trigger = triggers.size();
        for (u32 i = 0; i < triggers.size(); ++i) {
            if (triggers[i]) {
                assert (trigger == triggers.size());
                trigger = i;
            }
        }
        assert (trigger != triggers.size());
        // some more assertions
        assert (cycle_start == 1);
        assert (trigger == cycle_length - 1);
    }

    void find_cycle() {
        assert(state() == 0);
        std::unordered_map<u64, u64> cache;
        mockgoal *mgoal = new mockgoal(goal, {end});
        modul* oldgoal = modules[goal];
        modules[goal] = mgoal;
        while (1) {
            auto f = cache.find(state());
            if (f != cache.end()) {
                cycle_start = f->second;
                break;
            }
            cache[state()] = triggers.size();
            mgoal->triggered = false;
            push_start();
            triggers.push_back(mgoal->triggered);
        }
        modules[goal] = oldgoal;
        cycle_length = triggers.size() - cycle_start;
        assert_everything_is_good();
    }
};
u32 get_goal() {
    for (u32 u = 0; u < modules.size(); ++u) {
        for (u32 dest : modules[u]->dests)
            if (dest == U32MAX) return u;
    }
    return U32MAX;
}

void reset_modules() {
    for (auto ptr : modules)
        ptr->on = false;
}

u64 part2() {
    reset_modules();
    goal = get_goal();
    assert (goal != U32MAX);

    std::vector<analysis> a;

    std::set<u32> used;
    for (u32 u : modules[goal]->inputs) {
        u32 enclosures = 0;
        for (u32 v : modules[broadcaster]->dests) {
            auto enclosure = intersection(skipped(goal, u, true),
                                          skipped(broadcaster, v, false));
            if (enclosure.size() > 0) {
                ++enclosures;
                assert(!used.contains(v));
                used.insert(v);
                a.push_back(analysis{v, u, enclosure});
            }
        }
        assert (enclosures == 1);
    }

    u64 result = 1;
    for (auto &enc : a) result *= enc.cycle_length;
    return result;
}

int main () {
/*
_input = {
{"broadcaster",{"a"}},
{"%a",{ "inv", "con"}},
{"&inv",{ "b"}},
{"%b",{ "con" }},
{"&con",{"output"}}};
*/
    auto p1 = part1();
    std::cout << "Part 1: " << p1 << "\n";
    auto p2 = part2();
    std::cout << "Part 2: " << p2 << "\n";
    return 0;
}
