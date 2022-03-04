#include <iostream>
#include <set>
#include <vector>
#include <map>
#include <cmath>

int n, lg;
std::vector<int> p;
std::vector<int> d;  // dists
std::vector<std::vector<int>> dp;


int lca(int u, int v) {
  if (d[v] > d[u]) {
    std::swap(u, v);
  }
  for (int i = lg - 1; i >= 0; --i) {
    if (d[dp[u][i]] - d[v] >= 0) {
      u = dp[u][i];
    }
  }
  if (u == v) {
    return u;
  }
  for (int pow = lg - 1; pow >= 0; --pow) {
    if (dp[u][pow] != dp[v][pow]) {
      u = dp[u][pow];
      v = dp[v][pow];
    }
  }
  return p[u];
}


// solution for https://codeforces.com/group/QmrArgR1Jp/contest/279285/problem/B
int main() {
  std::cin.tie(nullptr);
  std::cout.tie(nullptr);
  std::ios_base::sync_with_stdio(false);

  std::cin >> n;
  p.resize(n, 0);
  d.resize(n, 0);

  // read parents
  p[0] = 0;
  d[0] = 0;
  for (int i = 1; i < n; ++i) {
    std::cin >> p[i];
    p[i]--;
    d[i] = d[p[i]] + 1;
  }

  lg = std::ceil(std::log2(n));
  dp.resize(n, std::vector<int> (lg, 0));  // 0-th vertex is root

  // preprocess
  for (int i = 0; i < n; ++i) {
    dp[i][0] = p[i];
  }

  for (int j = 1; j < lg; ++j) {
    for (int i = 0; i < n; ++i) {
      if (dp[i][j - 1] != -1) {
        dp[i][j] = dp[dp[i][j - 1]][j - 1];
      } else {
        dp[i][j] = -1;
      }
    }
  }

  int m;  // number of queries
  std::cin >> m;
  for (int i = 0; i < m; ++i) {
    int u, v;
    std::cin >> u >> v;
    u--, v--;
    std::cout << lca(u, v) + 1 << '\n';
  }

  return 0;
}