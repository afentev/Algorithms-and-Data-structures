#include <iostream>
#include <vector>


struct SparseTable {
  SparseTable(const std::vector<int>& a) {
    int n = a.size();
    int pw = 32 - __builtin_clz(n);
    table.resize(pw, std::vector<int> (n, 0));
    table[0] = a;
    for (int i = 0; i + 1 < pw; ++i) {
      int powered = 1 << i;
      for (int ind = 0; ind + powered < n; ++ind) {
        table[i + 1][ind] = std::min(table[i][ind], table[i][ind + powered]);
      }
    }
  }

  int query(int l, int r) {  // [l; r)
    int power = 31 - __builtin_clz(r - l);
    return std::min(table[power][l], table[power][r - (1u << power)]);
  }

  std::vector<std::vector<int>> table;
};


// solution for https://codeforces.com/gym/100093/problem/E
int32_t main() {
  freopen("sparse.in","r",stdin);
  freopen("sparse.out","w",stdout);

  std::ios_base::sync_with_stdio(false);
  std::cin.tie(nullptr);

  int n, m, f;
  std::cin >> n >> m >> f;
  std::vector<int> a(n, 0);
  a[0] = f;
  for (int i = 1; i < n; ++i) {
    a[i] = (23 * a[i - 1] + 21563) % 16714589;
  }

  SparseTable x(a);
  int u, v;
  std::cin >> u >> v;
  int ans;
  for (int i = 1; i <= m; ++i) {
    ans = x.query(std::min(u, v) - 1, std::max(v, u));
    if (i != m) {
      u = ((17 * u + 751 + ans + 2 * i) % n) + 1;
      v = ((13 * v + 593 + ans + 5 * i) % n) + 1;
    }
  }

  std::cout << u << ' ' << v << ' ' << ans << '\n';
  return 0;
}

