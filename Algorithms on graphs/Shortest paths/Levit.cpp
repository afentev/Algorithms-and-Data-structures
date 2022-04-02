#include <iostream>
#include <vector>
#include <tuple>
#include <unordered_set>
#include <cmath>
#include <queue>

struct Edge {
  int src, dst, w;

  bool operator< (const Edge& other) const {
    return std::make_tuple(src, dst, w) < std::make_tuple(other.src, other.dst, other.w);
  }
};

std::vector<int> dists;
std::vector<std::vector<Edge>> edges;

int n, m;
int inf = 30000;

void Levit(int vertex) {
  dists.resize(n, inf);
  dists[vertex] = 0;

  std::unordered_set<int> m0, m1, m2;
  m0.reserve(n); m0.max_load_factor(0.2);
  m1.reserve(n); m1.max_load_factor(0.2);
  m2.reserve(n); m2.max_load_factor(0.2);

  std::queue<int> m1Regular;
  std::queue<int> m1Fast;

  for (int i = 0; i < n; ++i) {
    if (i != vertex) {
      m2.insert(i);
    }
  }
  m1Regular.push(vertex);
  m1.insert(vertex);

  while (!m1.empty()) {
    int u;
    if (m1Fast.empty()) {
      u = m1Regular.front(); m1Regular.pop();
    } else {
      u = m1Fast.front(); m1Fast.pop();
    }
    m1.erase(u);
    for (const auto& edge: edges[u]) {
      int dst = edge.dst;
      if (m2.find(dst) != m2.end()) {
        m1Regular.push(dst);
        m1.insert(dst);
        m2.erase(dst);
        dists[dst] = dists[u] + edge.w;
      } else if (m1.find(dst) != m1.end()) {
        dists[dst] = std::min(dists[dst], dists[u] + edge.w);
      } else if (dists[dst] > dists[u] + edge.w) {
        m1Fast.push(dst);
        m1.insert(dst);
        m0.erase(dst);
        dists[dst] = dists[u] + edge.w;
      }
    }
    m0.insert(u);
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
    edges[u].push_back(Edge{u, v, w});
  }

  Levit(0);

  for (int i = 0; i < n; ++i) {
    std::cout << dists[i] << ' ';
  }
  std::cout  << '\n';

  return 0;
}
