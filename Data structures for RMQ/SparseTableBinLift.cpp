#include <iostream>
#include <vector>

struct SparseTable {
  static inline int neutral = 0;
  static int op(int arg1, int arg2) {
    return arg1 + arg2;
  }

  SparseTable(const std::vector<int>& a) {
    int n = a.size();
    int pw = 32 - __builtin_clz(n);
    table.resize(pw, std::vector<int> (n, 0));
    table[0] = a;
    for (int i = 0; i + 1 < pw; ++i) {
      int powered = 1 << i;
      for (int ind = 0; ind + powered < n; ++ind) {
        table[i + 1][ind] = op(table[i][ind], table[i][ind + powered]);
      }
    }
  }

  int query(int l, int r) {  // [l; r)
    int ans = neutral;
    for (int power = 31 - __builtin_clz(r - l); power >= 0; --power) {
      int length = 1 << power;
      if (l + length <= r) {
        ans = op(ans, table[power][l]);
        l += length;
      }
    }
    return ans;
  }

  std::vector<std::vector<int>> table;
};

// solution for https://codeforces.com/gym/100093/problem/D
int32_t main() {
  freopen("sum2.in","r",stdin);
  freopen("sum2.out","w",stdout);

  std::cin.tie(nullptr);
  std::ios_base::sync_with_stdio(false);

  int n;
  std::cin >> n;
  std::vector<int> a(n, 0);
  for (int i = 0; i < n; ++i) {
    std::cin >> a[i];
  }

  SparseTable x(a);

  int q;
  std::cin >> q;
  for (int i = 0; i < q; ++i) {
    int l, r;
    std::cin >> l >> r;
    std::cout << x.query(l - 1, r) << '\n';
  }

  return 0;
}

