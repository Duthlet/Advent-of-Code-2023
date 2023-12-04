#include <vector>
#include <iostream>
#include <string>

#include "input.inc"

std::vector<std::string> colors {
"red",
"blue",
"green"
};

bool check1 (std::string game) {
    // check only 12 red cubes, 13 green cubes, and 14 blue cubes
    std::vector<int> total {
    12,
    14,
    13
    };
    while (!game.empty()) {
        //read number
        size_t len, n;
        n = stoi(game, &len);
        game = game.substr(len + 1, std::string::npos);
        //read color
        for (int i = 0; i < 3; ++i) {
            len = colors[i].length();
            if (game.substr(0, len) == colors[i]) {
                if (n > total[i]) return false;
                game = game.substr(len, std::string::npos);
                break;
            }
        }
    }
    return true;
}

size_t power (std::string game) {
    std::vector<int> total {
    0,
    0,
    0
    };
    while (!game.empty()) {
        //read number
        size_t len, n;
        n = stoi(game, &len);
        game = game.substr(len + 1, std::string::npos);
        //read color
        for (int i = 0; i < 3; ++i) {
            len = colors[i].length();
            if (game.substr(0, len) == colors[i]) {
                if (n > total[i]) total[i] = n;
                game = game.substr(len, std::string::npos);
                break;
            }
        }
    }
    return total[0] * total[1] * total[2];
}

size_t part1() {
    size_t total = 0;
    for (int i = 0; i < input.size(); ++i) {
        if (check1(input[i])) total += i;
    }
    return total;
}

size_t part2() {
    size_t total = 0;
    for (int i = 0; i < input.size(); ++i) {
        total += power(input[i]);
    }
    return total;
}

int main () {

    std::cout << "Part 1: " << part1() << "\n";
    std::cout << "Part 2: " << part2() << "\n";
    return 0;
}
