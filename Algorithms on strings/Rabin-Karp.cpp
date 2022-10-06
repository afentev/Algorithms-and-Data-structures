#include <string>
#include <random>
#include <iostream>

const uint64_t m1 = (1ull << 31) - 1;

uint64_t modulo(uint64_t x) {
  x = x + 2147483647;
  x = (x >> 31) + (x & 2147483647);
  x = (x >> 31) + (x & 2147483647);
  return x;
}

// using double hashing
// m1 = 2^31 - 1
// m2 = 2^64
void RabinKarp(const std::string& text, const std::string& pattern) {
    if (text.size() < pattern.size()) {
        return;
    }
    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_int_distribution<uint64_t> dist1(0, m1 - 1);
    std::uniform_int_distribution<uint64_t> dist2(0, INT64_MAX - 1);
    uint64_t x1 = dist1(mt) + 1;
    uint64_t x2 = dist1(mt) + 1;

    uint64_t patternM1 = 0;
    uint64_t patternM2 = 0;
    uint64_t textM1 = 0;
    uint64_t textM2 = 0;
    uint64_t val1 = 1;
    uint64_t val2 = 1;
    for (int i = 0; i < pattern.size(); ++i) {
        patternM1 = modulo(patternM1 * x1 + pattern[i]);
        patternM2 = patternM2 * x2 + pattern[i];
        textM1 = modulo(textM1 * x1 + text[i]);
        textM2 = textM2 * x2 + text[i];
        val1 = modulo(val1 * x1);
        val2 = val2 * x2;
    }

    for (int i = 0; i <= text.size() - pattern.size(); ++i) {
        if (i != 0) {
            textM1 = modulo(x1 * textM1 - val1 * text[i - 1] + text[pattern.size() + i - 1]);
            textM2 = x2 * textM2 - val2 * text[i - 1] + text[pattern.size() + i - 1];
        }
        if (patternM1 == textM1 && patternM2 == textM2) {
            bool find = true;
            for (int j = 0; j < pattern.size(); ++j) {
                if (text[i + j] != pattern[j]) {
                    find = false;
                    break;
                }
            }
            if (find) {
                std::cout << i << ' ';
            }
        }
    }
}

// solution for https://codeforces.com/group/PVbQ8eK2T4/contest/400596/problem/A
int main() {
    std::cin.tie(nullptr);
    std::ios_base::sync_with_stdio(false);

    std::string s1, s2;
    std::cin >> s1 >> s2;
    RabinKarp(s1, s2);
}