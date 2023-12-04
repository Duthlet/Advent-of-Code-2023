#include <vector>
#include <iostream>
#include <string>

#include "input.inc"


bool contains_symbols(std::vector<int> symbols, int start, int end) {
    for (int i : symbols) {
        if (i >= start) return i <= end;
    }
    return false;
}

int part1() {
    std::vector<std::vector<int>> symbols;

    for (auto &line : input) {
        symbols.push_back(std::vector<int>{});
        for (int i = 0; i < line.length(); ++i) {
            if (line[i] != '.' && !(line[i] >= '0' && line[i] <= '9'))
                symbols.back().push_back(i);
        }
    }

    int sum = 0;

    for (int l = 0; l < input.size(); ++l) {
        const char *line = input[l].c_str();
        for (int i = 0; line[i] != '\0'; ++i) {
            if (line[i] >= '0' && line[i] <= '9') {
                char* next;
                int n;
                n = strtol(line+i, &next, 10);

                if (n == 0) std::cout << "found number " << n << "\n";

                int start = i - 1;
                i = next - line - 1;
                int end = i + 1;

                if (start < 0) start = 0;
                if (end >= input[l].length()) end = input[l].length() - 1;

                bool is_part_number = false;
                if (l != 0)
                    is_part_number = contains_symbols(symbols[l-1], start, end);
                if (!is_part_number)
                    is_part_number = contains_symbols(symbols[l], start, end);
                if (!is_part_number && (l+1 < input.size()))
                    is_part_number = contains_symbols(symbols[l+1], start, end);

                if (is_part_number)
                    sum += n;
            }
        }
    }
    return sum;
}

int64_t gear_number (const char* line, int i)
{
    if ((i != 0) && (line[i-1] >= '0' && line[i-1] <= '9')) {
        int j;
        for (j = i-1; j >= 0; --j) {
            if (!std::isdigit(line[j])) break;
        }
        ++j;

        char* next;
        int64_t n;
        n = strtoll(line+j, &next, 10);

        j = next - line;
        if ((j == i) && std::isdigit(line[i+1])){
            n *= strtoll(line+j+1, &next, 10);
            return -n;
        }
        return n;
    }
    if (std::isdigit(line[i])) {
        return strtoll(line + i, nullptr, 10);
    }
    if (std::isdigit(line[i+1])) {
        return strtoll(line + i + 1, nullptr, 10);
    }
    return 0;
}

int64_t part2() {

    int64_t sum = 0;
    for (int l = 0; l < input.size(); ++l) {
        auto &line = input[l];
        for (int i = 0; i < line.length(); ++i) {
            if (line[i] == '*') {
                int conn = 0;
                int64_t gear = 1;
                if (l != 0) {
                    int64_t n = gear_number(input[l - 1].c_str(), i);
                    if (n < 0) {
                        gear = -n;
                        conn += 2;
                    } else if (n){
                        gear *=n;
                        ++conn;
                    }
                }
                {
                    int64_t n = gear_number(input[l].c_str(), i);
                    if (n < 0) {
                        gear = -n;
                        conn += 2;
                    } else if (n){
                        gear *=n;
                        ++conn;
                    }
                }
                if (l+1 < input.size()) {
                    int64_t n = gear_number(input[l + 1].c_str(), i);
                    if (n < 0) {
                        gear = -n;
                        conn += 2;
                    } else if (n){
                        gear *=n;
                        ++conn;
                    }
                }
                if (conn == 2) sum += gear;
            }
        }
    }
    return sum;
}

int main () {
    auto p1 = part1();
    auto p2 = part2();
    std::cout << "Part 1: " << p1 << "\n";
    std::cout << "Part 2: " << p2 << "\n";
    return 0;
}
