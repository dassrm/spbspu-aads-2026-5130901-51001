#include <iostream>
#include <climits>
#include <cerrno>
#include <cctype>
#include <cstdlib>

#include "sequences.hpp"

int main() {
    smirnova::Sequences seqs;
    std::string name;
    std::string token;

    while (std::cin >> name) {
        smirnova::Numbers nums;
        while (std::cin >> token) {
            bool isNum = true;
            for (char c : token) {
                if (!isdigit(c)) {
                    isNum = false;
                    break;
                }
            }
            if (!isNum) {
                for (int i = token.length() - 1; i >= 0; --i) {
                    std::cin.putback(token[i]);
                }
                break;
            }

            char* endptr;
            errno = 0;
            long long num = std::strtoll(token.c_str(), &endptr, 10);
            if (errno == ERANGE || *endptr != '\0') {
                std::cerr << "error: overflow in sum calculation\n";
                return 1;
            }
            nums.pushBack(num);
        }
        if (!std::cin.eof()) {
            std::cin.clear();
        }
        seqs.pushBack(std::make_pair(name, std::move(nums)));
    }

    if (seqs.empty()) {
        std::cout << 0 << '\n';
        return 0;
    }

    bool first = true;
    for (auto it = seqs.cbegin(); it != seqs.cend(); ++it) {
        if (!first) std::cout << ' ';
        std::cout << it->first;
        first = false;
    }
    std::cout << '\n';

    smirnova::List<smirnova::LCIter<long long>> iters;
    for (auto it = seqs.cbegin(); it != seqs.cend(); ++it) {
        iters.pushBack(it->second.cbegin());
    }

    smirnova::Transposed transposed;
    while (true) {
        smirnova::Numbers row;
        auto seqIt = seqs.cbegin();
        auto iterIt = iters.begin();
        bool hasData = false;
        while (seqIt != seqs.cend()) {
            if (*iterIt != seqIt->second.cend()) {
                row.pushBack(**iterIt);
                ++(*iterIt);
                hasData = true;
            }
            ++seqIt;
            ++iterIt;
        }
        if (!hasData) break;
        transposed.pushBack(std::move(row));
    }

    if (transposed.empty()) {
        std::cout << 0 << '\n';
        return 0;
    }

    for (auto rowIt = transposed.cbegin(); rowIt != transposed.cend(); ++rowIt) {
        first = true;
        for (auto numIt = rowIt->cbegin(); numIt != rowIt->cend(); ++numIt) {
            if (!first) std::cout << ' ';
            std::cout << *numIt;
            first = false;
        }
        std::cout << '\n';
    }

    first = true;
    for (auto rowIt = transposed.cbegin(); rowIt != transposed.cend(); ++rowIt) {
        if (!first) std::cout << ' ';
        long long sum = 0;
        for (auto numIt = rowIt->cbegin(); numIt != rowIt->cend(); ++numIt) {
            if (*numIt > 0 && sum > LLONG_MAX - *numIt) {
                std::cerr << "error: overflow in sum calculation\n";
                return 1;
            }
            if (*numIt < 0 && sum < LLONG_MIN - *numIt) {
                std::cerr << "error: overflow in sum calculation\n";
                return 1;
            }
            sum += *numIt;
        }
        std::cout << sum;
        first = false;
    }
    std::cout << '\n';

    return 0;
}
