#include <iostream>
#include <vector>
#include <utility>
#include <set>
#include <map>

std::vector<int> dists;
std::vector<std::map<int, int>> g;
int n, m;

// O((n + m)*log(n))
void dijkstra(int v) {
  dists.resize(n, INT32_MAX);
  dists[v] = 0;

  std::set<std::pair<int, int>> unprocessed;
  for (int i = 0; i < n; ++i) {
    if (i != v) {
      unprocessed.insert({INT32_MAX, i});
    } else {
      unprocessed.insert({0, i});
    }
  }
  while (!unprocessed.empty()) {
    auto pair = *unprocessed.begin();
    int dist = pair.first;
    int vertex = pair.second;
    for (const std::pair<int, int>& lookup: g[vertex]) {
      if (lookup.second + dist < dists[lookup.first]) {
        unprocessed.erase({dists[lookup.first], lookup.first});
        dists[lookup.first] = lookup.second + dist;
        unprocessed.insert({dists[lookup.first], lookup.first});
      }
    }
    unprocessed.erase(pair);
  }
}

// solution for https://codeforces.com/group/QmrArgR1Jp/contest/302599/problem/B
// Find the shortest distance from the first vertex to all others.
int main() {
  std::cin >> n >> m;
  g.resize(n);
  for (int i = 0; i < m; ++i) {
    int u, v, w;
    std::cin >> u >> v >> w;
    u--, v--;
    g[u][v] = w;
    g[v][u] = w;
  }

  dijkstra(0);

  for (int i = 0; i < n; ++i) {
    std::cout << dists[i] << ' ';
  }
  std::cout << std::endl;

  return 0;
}
