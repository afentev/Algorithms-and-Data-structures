#include <iostream>
#include <vector>
#include <utility>
#include <set>

std::vector<int> dists;
std::vector<std::vector<std::pair<int, int>>> g;
int n, m;

// O((n + m)*log(n))
void dijkstra(int v) {
  dists.resize(n, INT32_MAX);
  dists[v] = 0;

  std::set<std::pair<int, int>> q;
  q.insert({0, v});
  while (!q.empty()) {
    auto pair = *q.begin();
    q.erase(q.begin());
    int dist = pair.first;
    int vertex = pair.second;
    for (const std::pair<int, int>& lookup: g[vertex]) {
      if (lookup.second + dist < dists[lookup.first]) {
        q.erase({dists[lookup.first], lookup.first});
        dists[lookup.first] = lookup.second + dist;
        q.insert({dists[lookup.first], lookup.first});
      }
    }
  }
}

// O(m + n*log(n))
#include <ext/pb_ds/priority_queue.hpp>
void dijkstraFibHeap(int v) {
  dists = std::vector<int>(n, INT32_MAX);
  dists[v] = 0;

  // we use pairing heap instead of fibonacci heap for performance reasons
  // we also could have used thin_heap_tag for thin heap
  __gnu_pbds::priority_queue<std::pair<int, int>, std::greater<>, __gnu_pbds::pairing_heap_tag> q;
  using iterator = __gnu_pbds::priority_queue<std::pair<int, int>, std::greater<>, __gnu_pbds::pairing_heap_tag>::point_iterator;
  std::vector<iterator> table;
  table.reserve(n);

  for (int i = 0; i < n; ++i) {
    if (i != v) {
      table.push_back(q.push({INT32_MAX, i}));
    } else {
      table.push_back(q.push({0, i}));
    }
  }
  while (!q.empty()) {
    auto pair = q.top();
    q.erase(table[pair.second]);
    int dist = pair.first;
    int vertex = pair.second;
    for (const auto& lookup: g[vertex]) {
      if (lookup.second + dist < dists[lookup.first]) {
        dists[lookup.first] = lookup.second + dist;
        q.modify(table[lookup.first], {dists[lookup.first], lookup.first});
      }
    }
  }
}

// solution for https://codeforces.com/group/QmrArgR1Jp/contest/302599/problem/B
// Find the shortest distance from the first vertex to all others.
int main() {
  std::cin.tie(nullptr);
  std::cout.tie(nullptr);
  std::ios_base::sync_with_stdio(false);

  std::cin >> n >> m;
  g.resize(n);
  for (int i = 0; i < m; ++i) {
    int u, v, w;
    std::cin >> u >> v >> w;
    u--, v--;
    g[u].emplace_back(v, w);
    g[v].emplace_back(u, w);
  }

  dijkstra(0);

  for (int i = 0; i < n; ++i) {
    std::cout << dists[i] << ' ';
  }
  std::cout << std::endl;

  return 0;
}