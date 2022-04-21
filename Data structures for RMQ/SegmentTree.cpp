#include <iostream>
#include <vector>

#define int int64_t


struct SegmentTree {
  static inline int neutral = 0;
  int op(int arg1, int arg2) {
    return arg1 + arg2;
  }

  SegmentTree(const std::vector<int>& a) {
    n = getPower(a.size());
    t.resize(n << 1, 0);
    build(a);
  }

  void build(const std::vector<int>& a) {
    for (int i = 0; i < a.size(); ++i) {
      t[n + i] = a[i];
    }
    for (int i = n - 1; i > 0; --i) {
      t[i] = op(t[i << 1], t[(i << 1) | 1]);
    }
  }

  int query(int l, int r) {  // [l; r)
    l += n;
    r += n;
    int ans = neutral;
    while (l < r) {
      if (l & 1) {
        ans = op(ans, t[l++]);
      }
      if (r & 1) {
        ans = op(ans, t[--r]);
      }
      l >>= 1;
      r >>= 1;
    }
    return ans;
  }

  void update(int pos, int newVal) {
    pos += n;
    t[pos] = newVal;
    pos >>= 1;
    while (pos > 0) {
      t[pos] = op(t[pos << 1], t[(pos << 1) | 1]);
      pos >>= 1;
    }
  }

  static int getPower(int n) {
    int ans = 1;
    while (ans < n) {
      ans <<= 1;
    }
    return ans;
  }

  std::vector<int> t;
  int n = 0;
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

  SegmentTree tree(a);

  std::string command;
  while (std::cin >> command) {
    if (command == "sum") {
      int l, r;
      std::cin >> l >> r;
      --l;
      std::cout << tree.query(l, r) << '\n';
    } else {
      int index, newVal;
      std::cin >> index >> newVal;
      --index;
      tree.update(index, newVal);
    }
  }

  return 0;
}
