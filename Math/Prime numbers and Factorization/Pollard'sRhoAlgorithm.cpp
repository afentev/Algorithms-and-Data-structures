#include <iostream>
#include <cmath>

int64_t gcd(int64_t a, int64_t b) {
  while (b) {
    a %= b;
    std::swap(a, b);
  }
  return a;
}

// finds some divisor of n
int64_t rhoPollard(int64_t n) {
  int64_t slow = 2;
  int64_t fast = 2;
  int64_t d = 1;
  while (d == 1) {
    slow = (slow * slow + 1) % n;
    fast = (fast * fast + 1) % n;
    fast = (fast * fast + 1) % n;
    d = gcd(n, std::abs(fast - slow));
  }
  return d;
}

int32_t main() {
  int64_t i = 614657ll*909091ll;
  while (true) {
    std::cout << i << ' ' << rhoPollard(i) << '\n';
    ++i;
  }
}
