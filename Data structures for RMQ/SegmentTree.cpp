#include <iostream>
#include <vector>

#define int int64_t


struct SegmentTree {
  static inline int neutral = 0;
  int op(int arg1, int arg2) {
    return arg1 + arg2;
  }

  SegmentTree(const std::vector<int>& a) {
    n = a.size();
    t.resize(n * 4, 0);
    build(a, 1, 0, n - 1);
  }

  void build(const std::vector<int>& a, int i, int lSubSeg, int rSubSeg) {
    if (rSubSeg == lSubSeg == 1) {
      t[i] = a[lSubSeg];
    } else {
      int mid = (lSubSeg + rSubSeg) / 2;
      build(a, 2 * i, lSubSeg, mid);
      build(a, 2 * i + 1, mid + 1, rSubSeg);
      t[i] = op(t[2 * i], t[2 * i + 1]);
    }
  }

  int query(int l, int r) {
    return query(1, 0, n - 1, l, r);
  }

  int query(int i, int lSubSeg, int rSubSeg, int lQuery, int rQuery) {
    if (lQuery > rSubSeg || rQuery < lSubSeg) {
      return neutral;
    }
    if (lQuery <= lSubSeg && rSubSeg <= rQuery) {
      return t[i];
    }
    int mid = (lSubSeg + rSubSeg) / 2;
    int left = query(2 * i, lSubSeg, mid, lQuery, rQuery);
    int right = query(2 * i + 1, mid + 1, rSubSeg, lQuery, rQuery);

    return op(left, right);
  }

  void update(int i, int newVal) {
    update(1, 0, n - 1, i, newVal);
  }

  void update(int i, int lSubSeg, int rSubSeg, int index, int newVal) {
    if (lSubSeg == rSubSeg) {
      t[i] = newVal;
    } else {
      int mid = (lSubSeg + rSubSeg) / 2;
      if (index <= mid) {
        update(2 * i, lSubSeg, mid, index, newVal);
      } else {
        update(2 * i + 1, mid + 1, rSubSeg, index, newVal);
      }

      t[i] = op(t[2 * i], t[2 * i + 1]);
    }
  }

  std::vector<int> t;
  int n = 0;
};


// solution for https://codeforces.com/group/QmrArgR1Jp/contest/269449/problem/B
int32_t main() {
  std::cin.tie(nullptr);
  std::cout.tie(nullptr);
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
      --l, --r;
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

