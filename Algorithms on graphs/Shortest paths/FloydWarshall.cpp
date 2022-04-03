#include <iostream>
#include <vector>

std::vector<std::vector<int>> g;
std::vector<std::vector<int>> dists;

int n;
int inf = INT32_MAX - 1;

void FloydWarshall() {
  for (int stage = 0; stage < n; ++stage) {
    for (int i = 0; i < n; ++i) {
      for (int j = 0; j < n; ++j) {
        if (dists[i][stage] < inf && dists[stage][j] < inf) {
          dists[i][j] = std::min(dists[i][j], dists[i][stage] + dists[stage][j]);
        }
      }
    }
  }
}

// solution for https://codeforces.com/group/QmrArgR1Jp/contest/302599/problem/A
int32_t main() {
  std::cin.tie(nullptr);
  std::ios_base::sync_with_stdio(false);

  std::cin >> n;
  g.resize(n, std::vector<int>(n, inf));
  dists.resize(n, std::vector<int>(n, inf));
  for (int i = 0; i < n; ++i) {
    for (int j = 0; j < n; ++j) {
      int w;
      std::cin >> w;
      dists[i][j] = g[i][j] = w;
    }
  }

  FloydWarshall();

  for (int i = 0; i < n; ++i) {
    for (int j = 0; j < n; ++j) {
      std::cout << dists[i][j] << ' ';
    }
    std::cout << '\n';
  }

  return 0;
}
