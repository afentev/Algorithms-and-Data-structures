//
// Created by afentev on 2/23/22.
//
#include <iostream>
#include <vector>
#include <set>

#include <ext/pb_ds/priority_queue.hpp>

int n, m;
int64_t weight = 0;
std::vector<std::vector<std::pair<int, int>>> g;


// O(E*log(V))
void prim() {
  std::vector<int> dists(n, INT32_MAX);  // dists[v] == -1 means that v is already included
  dists[0] = 0;

  std::set<std::pair<int, int>> q;  // weight, v
  for (const auto& x: g[0]) {
    q.insert({x.second, x.first});
    dists[x.first] = x.second;
  }
  while (!q.empty()) {
    auto pair = *q.begin();
    q.erase(q.begin());
    if (dists[pair.second] == -1) {
      continue;
    }
    weight += pair.first;
    dists[pair.second] = -1;

    for (const auto& edge: g[pair.second]) {
      if (dists[edge.first] > edge.second) {
        q.erase({dists[edge.first], edge.first});
        dists[edge.first] = edge.second;
        q.insert({dists[edge.first], edge.first});
      }
    }
  }
}


// O(V*log(V)+E)
void primFibHeap() {
  __gnu_pbds::priority_queue<std::pair<int, int>, std::greater<>, __gnu_pbds::pairing_heap_tag> q;
  using iterator = __gnu_pbds::priority_queue<std::pair<int, int>, std::greater<>, __gnu_pbds::pairing_heap_tag>::point_iterator;

  std::vector<iterator> table(n, nullptr);
  std::vector<int> dists(n, INT32_MAX);  // dists[v] == -1 means that v is already included
  dists[0] = 0;

  for (const auto& x: g[0]) {
    table[x.first] = q.push({x.second, x.first});
    dists[x.first] = x.second;
  }
  while (!q.empty()) {
    auto pair = q.top(); q.pop();
    if (dists[pair.second] == -1) {
      continue;
    }
    weight += pair.first;
    dists[pair.second] = -1;

    for (const auto& edge: g[pair.second]) {
      if (dists[edge.first] > edge.second) {
        if (dists[edge.first] == INT32_MAX) {
          table[edge.first] = q.push({dists[edge.first] = edge.second, edge.first});
        } else {
          q.modify(table[edge.first], {dists[edge.first] = edge.second, edge.first});
        }
      }
    }
  }
}


// solution for https://codeforces.com/group/QmrArgR1Jp/contest/298379/problem/J
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

  prim();

  std::cout << weight << '\n';
}