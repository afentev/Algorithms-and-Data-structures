#include <iostream>
#include <vector>

#define int int64_t


// segment tree with lazy propagation
// in current implementation: minima on a segment, set on a segment, addition on a segment
struct SegmentTree {
  static inline int neutral = 2e18;
  int op(int arg1, int arg2) {
    return std::min(arg1, arg2);
  }

  SegmentTree(const std::vector<int>& a) {
    n = a.size();
    t.resize(n << 2, neutral);
    propagation.resize(n << 2, 0);
    propagateAdd.resize(n << 2, 0);
    propagateSet.resize(n << 2, 0);
    build(a, 1, 0, n - 1);
  }

  void build(const std::vector<int>& a, int i, int lSubSeg, int rSubSeg) {
    if (rSubSeg == lSubSeg == 1) {
      t[i] = a[lSubSeg];
    } else {
      int mid = (lSubSeg + rSubSeg) >> 1;
      build(a, i << 1, lSubSeg, mid);
      build(a, (i << 1) | 1, mid + 1, rSubSeg);
      t[i] = op(t[i << 1], t[(i << 1) | 1]);
    }
  }

  int query(int l, int r) {  // [l; r]
    return query(1, 0, n - 1, l, r);
  }

  int query(int i, int lSubSeg, int rSubSeg, int lQuery, int rQuery) {
    push(i, lSubSeg, rSubSeg);

    if (lQuery > rSubSeg || rQuery < lSubSeg) {
      return neutral;
    }
    if (lQuery <= lSubSeg && rSubSeg <= rQuery) {
      return t[i];
    }
    int mid = (lSubSeg + rSubSeg) >> 1;
    int left = query(i << 1, lSubSeg, mid, lQuery, rQuery);
    int right = query((i << 1) | 1, mid + 1, rSubSeg, lQuery, rQuery);

    return op(left, right);
  }

  void set(int pos, int newVal) {
    massSet(1, 0, n - 1, pos, pos, newVal);
  }

  void massAdd(int delta, int l, int r) {
    massAdd(1, 0, n - 1, l, r, delta);
  }

  void massAdd(int i, int lSubSeg, int rSubSeg, int lQuery, int rQuery, int delta) {
    push(i, lSubSeg, rSubSeg);

    if (lQuery > rSubSeg || rQuery < lSubSeg) {
      return;
    }
    int mid = (lSubSeg + rSubSeg) >> 1;
    if (lQuery <= lSubSeg && rSubSeg <= rQuery) {
      if (lSubSeg != rSubSeg) {
        if (propagation[i << 1] == 1) {
          push(i << 1, lSubSeg, mid);
          push((i << 1) | 1, mid + 1, rSubSeg);
        }
        propagation[(i << 1)] = propagation[(i << 1) | 1] = 2;
        propagateAdd[(i << 1)] += delta;
        propagateAdd[(i << 1) | 1] += delta;
      }
      t[i] += delta;
    } else {
      massAdd(i << 1, lSubSeg, mid, lQuery, rQuery, delta);
      massAdd((i << 1) | 1, mid + 1, rSubSeg, lQuery, rQuery, delta);
      t[i] = op(t[i << 1], t[(i << 1) | 1]);
    }
  }

  void massSet(int newVal, int l, int r) {
    massSet(1, 0, n - 1, l, r, newVal);
  }

  void massSet(int i, int lSubSeg, int rSubSeg, int lQuery, int rQuery, int newVal) {
    push(i, lSubSeg, rSubSeg);

    if (lQuery > rSubSeg || rQuery < lSubSeg) {
      return;
    }
    if (lQuery <= lSubSeg && rSubSeg <= rQuery) {
      if (lSubSeg != rSubSeg) {
        propagation[(i << 1)] = propagation[(i << 1) | 1] = 1;
        propagateSet[(i << 1)] = propagateSet[(i << 1) | 1] = newVal;
        propagateAdd[i << 1] = propagateAdd[(i << 1) | 1] = 0;
      }
      t[i] = newVal;
    } else {
      int mid = (lSubSeg + rSubSeg) >> 1;
      massSet(i << 1, lSubSeg, mid, lQuery, rQuery, newVal);
      massSet((i << 1) | 1, mid + 1, rSubSeg, lQuery, rQuery, newVal);
      t[i] = op(t[i << 1], t[(i << 1) | 1]);
    }
  }

  void push(int pos, int l, int r) {
    bool isLeaf = l == r;
    int mid = (l + r) >> 1;

    if (propagation[pos] == 0) {
      return;
    }
    if (propagation[pos] == 1) {
      t[pos] = propagateSet[pos];
      if (!isLeaf) {
        propagateSet[pos << 1] = propagateSet[(pos << 1) | 1] = propagateSet[pos];
        propagateAdd[pos << 1] = propagateAdd[(pos << 1) | 1] = propagateAdd[pos] = 0;
      }
    } else {
      t[pos] += propagateAdd[pos];
      if (!isLeaf) {
        if (propagation[pos << 1] == 1) {
          push(pos << 1, l, mid);
          push((pos << 1) | 1, mid + 1, r);
        }
        propagateAdd[pos << 1] += propagateAdd[pos];
        propagateAdd[(pos << 1) | 1] += propagateAdd[pos];
      }
      propagateAdd[pos] = 0;
    }
    if (!isLeaf) {
      propagation[pos << 1] = propagation[(pos << 1) | 1] = propagation[pos];
    }
    propagation[pos] = 0;
  }

  std::vector<int> t;
  std::vector<int> propagateSet;
  std::vector<int> propagateAdd;
  std::vector<char> propagation;  // 0 - no propagation, 1 - last set, 2 - last add
  int n = 0;
};


// solution for https://codeforces.com/group/QmrArgR1Jp/contest/269449/problem/C
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
    if (command == "min") {
      int l, r;
      std::cin >> l >> r;
      --l, --r;
      std::cout << tree.query(l, r) << '\n';
    } else if (command == "set") {
      int newVal, l, r;
      std::cin >> l >> r >> newVal;
      --l, --r;
      tree.massSet(newVal, l, r);
    } else {
      int delta, l, r;
      std::cin >> l >> r >> delta;
      --l, --r;
      tree.massAdd(delta, l, r);
    }
  }

  return 0;
}
