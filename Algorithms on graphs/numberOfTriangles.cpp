#include <iostream>
#include <vector>


int n, m;
std::vector<std::vector<int>> g;
std::vector<std::vector<int>> hg;
int triangles = 0;

// complexity is O(E√E)
void countTriangles() {
  for (int v = 0; v < n; ++v) {
    std::copy_if(
        std::begin(g[v]),
        std::end(g[v]),
        std::back_inserter(hg[v]),
        [v](int a) { return !(g[v].size() < g[a].size() || (g[v].size() == g[a].size() && v < a)); }
    );
  }

  std::vector<int> cnt(n, 0);
  for (int u = 0; u < n; ++u) {
    for (int w: hg[u]) {
      cnt[w] = 0;
    }
    for (int v: hg[u]) {
      for (int w: hg[v]) {
        ++cnt[w];
      }
    }
    for (int w: hg[u]) {
      triangles += cnt[w];
    }
  }
}

// solution for https://codeforces.com/group/NOwIbqv33y/contest/307527/problem/A
int main() {
  std::cin.tie(nullptr);
  std::cout.tie(nullptr);
  std::ios_base::sync_with_stdio(false);

  std::cin >> n >> m;
  g.resize(n, std::vector<int>());
  hg.resize(n, std::vector<int>());

  for (int i = 0; i < m; ++i) {  // unoriented graph
    int u, v;
    std::cin >> u >> v;
    u--, v--;

    g[u].push_back(v);
    g[v].push_back(u);
  }

  countTriangles();

  std::cout << triangles << std::endl;
}
