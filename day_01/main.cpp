#include <vector>
#include <iostream>
#include <string>
#include <cassert>

#include "input.inc"

std::vector<std::string> numbers = {
"one",
"two",
"three",
"four",
"five",
"six",
"seven",
"eight",
"nine"};

int val (std::string& str) {
    int first = -1;
    int last = -1;
    for (int i = 0; i < str.length(); ++i) {
        char c = str[i];
        if (c >= '0' && c <= '9') {
            if (first == -1) {
                first = c - '0';
            }
            last = c - '0';
        } else {
            for (int n = 1; n <= 9; ++n) {
                int len = numbers[n-1].length();
                if (str.substr(i, len) == numbers[n-1]) {
                    if (first == -1) {
                        first = n;
                    }
                    last = n;
                    //i += len - 1;
                }
            }
        }
    }
    assert (first != -1);
    assert (last != -1);
    return first*10 + last;
}

int main () {
    std::vector<int> vals {};

    for (auto &str : input) {
        vals.push_back(val(str));
    }

    int sum = 0;
    for (auto i : vals) {
        sum += i;
    }

    std::cout << sum << "\n";
    return 0;
}
