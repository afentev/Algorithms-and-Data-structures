#include <iostream>
#include <vector>

std::vector<size_t> primes;  // list of prime numbers
std::vector<size_t> divisors;  // the least prime number to divide i

void eratosthenes(size_t n) {
  divisors.resize(n + 1, 0);
  for (size_t i = 2; i <= n; ++i) {
    if (divisors[i] == 0) {
      primes.push_back(i);
      divisors[i] = i;
    }
    for (size_t j = 0; j < primes.size() && primes[j] <= divisors[i] && primes[j] * divisors[i] <= n; ++j) {
      divisors[primes[j] * i] = primes[j];
    }
  }
}

int main() {
  eratosthenes(100);
  for (const auto& x: primes) {
    std::cout << x << '\n';
  }
}