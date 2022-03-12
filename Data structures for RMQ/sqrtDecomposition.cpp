#include <iostream>
#include <vector>
#include <cmath>

#define int int64_t


class SqrtDecomposition {
  static inline int neutral = 0;
  static int op(int arg1, int arg2) {
    return arg1 + arg2;
  }

 public:
  SqrtDecomposition(const std::vector<int>& v) {
    array = v;
    sqrt = static_cast<size_t>(std::sqrt(v.size())) + 1;
    buckets.resize(sqrt, neutral);
    for (size_t i = 0; i < v.size(); ++i) {
      buckets[i / sqrt] = op(buckets[i / sqrt], v[i]);
    }
  }

  void update(int pos, int newVal) {
    array[pos] = newVal;
    size_t bucket = pos / sqrt;
    buckets[bucket] = neutral;
    for (size_t i = bucket * sqrt; i < std::min(array.size(), (bucket + 1) * sqrt); ++i) {
      buckets[bucket] = op(buckets[bucket], array[i]);
    }
  }

  int query(int l, int r) {  // [l; r]
    size_t leftBucket = l / sqrt;
    size_t rightBucket = r / sqrt;
    int ans = neutral;
    if (leftBucket == rightBucket) {
      for (size_t i = l; i <= r; ++i) {
        ans = op(ans, array[i]);
      }
    } else {
      for (size_t i = l; i < (leftBucket + 1) * sqrt; ++i) {
        ans = op(ans, array[i]);
      }
      for (size_t i = leftBucket + 1; i <= rightBucket - 1; ++i) {
        ans = op(ans, buckets[i]);
      }
      for (size_t i = rightBucket * sqrt; i <= r; ++i) {
        ans = op(ans, array[i]);
      }
    }
    return ans;
  }

 private:
  size_t sqrt;
  std::vector<int> array;
  std::vector<int> buckets;
};


// solution for https://codeforces.com/group/QmrArgR1Jp/contest/269449/problem/B
int32_t main() {
  std::cin.tie(nullptr);
  std::ios_base::sync_with_stdio(false);

  int n;
  std::cin >> n;

  std::vector<int> a(n, 0);
  for (int i = 0; i < n; ++i) {
    std::cin >> a[i];
  }

  SqrtDecomposition structure(a);

  std::string command;
  while (std::cin >> command) {
    if (command == "sum") {
      int l, r;
      std::cin >> l >> r;
      --l, --r;
      std::cout << structure.query(l, r) << '\n';
    } else {
      int index, newVal;
      std::cin >> index >> newVal;
      --index;
      structure.update(index, newVal);
    }
  }

  return 0;
}