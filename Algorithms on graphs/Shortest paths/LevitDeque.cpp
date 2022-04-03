#include <iostream>
#include <vector>
#include <deque>

struct Edge {
  int dst, w;
};

std::vector<int> dists;
std::vector<std::vector<Edge>> edges;

int n, m;
int inf = 30000;


// At worst-case scenario this implementation performs in O(2^n)
// There is an implementation with linear complexity: Levit.cpp
// But in the average this one works faster
// More info on this: https://codeforces.com/blog/entry/3793?locale=ru

void Levit(int vertex) {
  dists.resize(n, inf);
  dists[vertex] = 0;

  std::vector<int> labels(n, 2);

  std::deque<int> m1{vertex};

  while (!m1.empty()) {
    int u = m1.front(); m1.pop_front();
    labels[u] = 0;
    for (const auto& edge: edges[u]) {
      int dst = edge.dst;
      if (dists[dst] > dists[u] + edge.w) {
        dists[dst] = dists[u] + edge.w;
        if (labels[dst] == 2) {
          labels[dst] = 1;
          m1.push_back(dst);
        } else if (labels[dst] == 0) {
          labels[dst] = 1;
          m1.push_front(dst);
        }
      }
    }
  }
}


// solution for https://www.eolymp.com/ru/problems/1453
int32_t main() {
  std::cin.tie(nullptr);
  std::ios_base::sync_with_stdio(false);

  std::cin >> n >> m;
  edges.resize(n);
  for (int i = 0; i < m; ++i) {
    int u, v, w;
    std::cin >> u >> v >> w;
    u--, v--;
    edges[u].push_back(Edge{v, w});
  }

  Levit(0);

  for (int i = 0; i < n; ++i) {
    std::cout << dists[i] << ' ';
  }
  std::cout  << '\n';

  return 0;
}
