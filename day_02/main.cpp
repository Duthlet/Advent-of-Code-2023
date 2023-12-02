#include <vector>
#include <iostream>
#include <string>

#include "input.inc"

std::vector<std::string> colors {
"red",
"blue",
"green"
};
/*
std::vector<int> total {
12,
14,
13
};
*//*
bool check1 (std::string game) {
    // check only 12 red cubes, 13 green cubes, and 14 blue cubes
//    for (auto it = game.begin(); it != game.end(); ++it) {
    while (!game.empty()) {
        std::cout << game << "\n";
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
*/
size_t power (std::string game) {
    // check only 12 red cubes, 13 green cubes, and 14 blue cubes
//    for (auto it = game.begin(); it != game.end(); ++it) {
    std::vector<int> total {
    0,
    0,
    0
    };
    while (!game.empty()) {
        std::cout << game << "\n";
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
int main () {
    size_t total = 0;
    for (int i = 0; i < input.size(); ++i) {
//        if (check1(input[i]))
//            total += i;
        total += power(input[i]);
    }

    std::cout << total << "\n";
    return 0;
}
