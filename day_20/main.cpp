#include <vector>
#include <iostream>
#include <shortinttypes.h>
#include <string>
#include <cassert>
#include <unordered_map>
#include <queue>

#include "input.inc"

u64 lows = 0;
u64 highs = 0;

struct modul;

std::unordered_map<std::string, modul*> input;
modul* bc;

struct request {
    std::string sender;
    std::string receiver;
    bool high;
};

std::queue<request> requests;
bool handle_request();

struct modul {
    virtual void low_pulse(const std::string& sender) = 0;
    virtual void high_pulse(const std::string& sender) = 0;
    virtual void get_input(const std::string& sender) {}
    std::string name;
    std::vector<std::string> destinations;

    modul (std::string name, std::vector<std::string> d) : name(name), destinations(d) {}

    void send_input() {
        for (auto s : destinations) {
            if (input.find(s) == input.end()) continue;
            input[s]->get_input(name);
        }
    }
    void send_high(){
        for (auto s : destinations) requests.push({name, s, true});
        while(handle_request());
    }
    void send_low() {
        for (auto s : destinations) requests.push({name, s, false});
        while(handle_request());
    }

    virtual ~modul() = default;
};

struct broadcast : modul {
    broadcast (std::string name, std::vector<std::string> d) : modul(name, d) {}
    void low_pulse(const std::string& sender) override {
        send_low();
    }
    void high_pulse(const std::string& sender) override {
        send_high();
    }
};

struct flipflop : modul {
    bool on;

    flipflop (std::string name, std::vector<std::string> d)
        : modul(name, d), on(false) {}

    void low_pulse(const std::string& sender) override {
        on = !on;
        if (on) send_high();
        else send_low();
    }
    void high_pulse(const std::string& sender) override {
    }
};

struct conjunction : modul {
    std::unordered_map<std::string, bool> inputs_received;

    conjunction (std::string name, std::vector<std::string> d) : modul(name, d) {}

    void get_input(const std::string& sender) override {
        inputs_received[sender] = false;
    }

    bool all_true () {
        for (const auto &p : inputs_received) {
            if (!p.second) return false;
        }
        return true;
    }

    void low_pulse(const std::string& sender) override {
        inputs_received[sender] = false;
        all_true() ? send_low() : send_high();
    }
    void high_pulse(const std::string& sender) override {
        inputs_received[sender] = true;
        all_true() ? send_low() : send_high();
    }
};

bool handle_request () {
    if (requests.empty()) return false;
    auto& r = requests.front();
    requests.pop();
    auto f = input.find(r.receiver);
    if (r.high) {
        ++highs;
        if (f != input.end()) f->second->high_pulse(r.sender);
    } else {
        ++lows;
        if (f != input.end()) f->second->low_pulse(r.sender);
    }
    return true;
}

void push_button () {
    assert (requests.empty());
    ++lows;
    input["broadcaster"]->low_pulse("");
}

void create_input() {
    for (const auto& p : _input) {
        switch(p.first[0]) {
            case '%': {
                std::string name = p.first.substr(1, std::string::npos);
                input[name] = new flipflop(name, p.second);
                break;}
            case '&': {
                std::string name = p.first.substr(1, std::string::npos);
                input[name] = new conjunction(name, p.second);
                break;}
            default:
                assert (p.first == "broadcaster");
                const std::string& name = p.first;
                input[name] = new broadcast(name, p.second);
                bc = input[name];
        }
    }
    for (const auto& p: input) {
        p.second->send_input();
    }
}
u64 part1() {
    create_input();
    for (u32 i = 0; i < 1000; ++i) {
        push_button();
    }
    return highs * lows;
}

u64 part2() {
    return 0;
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
    std::cout << lows << " lows\n" << highs << " highs\n";
    std::cout << "Part 1: " << p1 << "\n";
    auto p2 = part2();
    std::cout << "Part 2: " << p2 << "\n";
    return 0;
}
