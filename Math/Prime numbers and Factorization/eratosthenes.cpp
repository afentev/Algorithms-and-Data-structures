#include <iostream>
#include <vector>

// char is used in order to avoid vector<bool>
std::vector<char> isPrime;
// std::vector<char> isPrime;

void eratosthenes(size_t n) {
  isPrime.resize(n + 1, true);
  for (size_t i = 2; i * i <= n; ++i) {
    if (isPrime[i]) {
      for (size_t j = i * i; j <= n; j += i) {
        isPrime[j] = false;
      }
    }
  }
}

int main() {
  eratosthenes(100);
  for (int i = 2; i <= 100; ++i) {
    std::cout << i << ' ' << (isPrime[i] != '\0') << '\n';
  }
}