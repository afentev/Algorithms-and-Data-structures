#include <iostream>
#include <vector>

#define int int64_t

std::vector<int> a;


struct FenwickTree {
  FenwickTree(const std::vector<int>& a) {
    t.resize(a.size(), 0);
    for (int i = 0; i < a.size(); ++i) {
      inc(i, a[i]);
    }
  }

  int sum(int l, int r) {
    return sum(r) - sum(l - 1);
  }

  int sum(int r) {
    int result = 0;
    for (; r >= 0; r = (r & (r + 1)) - 1) {
      result += t[r];
    }
    return result;
  }

  void inc(int pos, int delta) {
    for (; pos < t.size(); pos = pos | (pos + 1)) {
      t[pos] += delta;
    }
  }

  void update(int pos, int newVal) {
    inc(pos, newVal - a[pos]);
    a[pos] = newVal;
  }
  std::vector<int> t;
};


// solution for https://codeforces.com/group/QmrArgR1Jp/contest/269449/problem/B
int32_t main() {
  std::cin.tie(nullptr);
  std::ios_base::sync_with_stdio(false);

  int n;
  std::cin >> n;

  a.resize(n, 0);
  for (int i = 0; i < n; ++i) {
    std::cin >> a[i];
  }

  FenwickTree tree(a);

  std::string command;
  while (std::cin >> command) {
    if (command == "sum") {
      int l, r;
      std::cin >> l >> r;
      --l, --r;
      std::cout << tree.sum(l, r) << '\n';
    } else {
      int index, newVal;
      std::cin >> index >> newVal;
      --index;
      tree.update(index, newVal);
    }
  }

  return 0;
}

