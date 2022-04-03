#include <iostream>
#include <vector>
#include <queue>

struct Edge {
  int dst, w;
};

struct Label {
  char m1, m2;
};

std::vector<int> dists;
std::vector<std::vector<Edge>> edges;

int n, m;
int inf = 30000;

void Levit(int vertex) {
  dists.resize(n, inf);
  dists[vertex] = 0;

  std::vector<Label> labels(n, Label{0, 1});

  std::queue<int> m1Regular;
  std::queue<int> m1Fast;

  m1Regular.push(vertex);
  labels[vertex] = {1, 0};

  while (!m1Regular.empty() || !m1Fast.empty()) {
    int u;
    if (m1Fast.empty()) {
      u = m1Regular.front(); m1Regular.pop();
    } else {
      u = m1Fast.front(); m1Fast.pop();
    }
    labels[u].m1 = false;
    for (const auto& edge: edges[u]) {
      int dst = edge.dst;
      if (labels[dst].m2) {
        m1Regular.push(dst);
        labels[dst].m1 = true;
        labels[dst].m2 = false;
        dists[dst] = dists[u] + edge.w;
      } else if (labels[dst].m1) {
        dists[dst] = std::min(dists[dst], dists[u] + edge.w);
      } else if (dists[dst] > dists[u] + edge.w) {
        m1Fast.push(dst);
        labels[dst].m1 = true;
        dists[dst] = dists[u] + edge.w;
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
