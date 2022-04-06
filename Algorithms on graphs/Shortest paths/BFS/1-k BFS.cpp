#include <iostream>
#include <queue>
#include <vector>
#include <map>

const int INF = 1e9;
int k = -INF;
int n, m;

struct Edge {
  int dst, w;
};

std::vector<std::vector<Edge>> g;
std::vector<int> dist;

void bfs1k(int vertex) {
  std::vector<std::queue<int>> ar(k + 1, std::queue<int>());
  ar[0].push(vertex);

  int queue = 0;
  for (int i = 0; i < k * n; ++i) {
    while (!ar[queue].empty()) {
      int u = ar[queue].front(); ar[queue].pop();
      if (dist[u] != i) {
        continue;
      }
      for (const auto& v: g[u]) {
        if (dist[u] + v.w < dist[v.dst]) {
          dist[v.dst] = dist[u] + v.w;
          ar[(queue + v.w) % (k + 1)].push(v.dst);
        }
      }

    }
    queue = (queue + 1) % (k + 1);
  }
}


/*
 * solution for https://silvertests-mes.mos.ru/OlympTask.aspx?id=21770
 * O(V*K + E) time complexity
 */
int main() {
  std::cin >> n >> m;
  g.resize(n, std::vector<Edge>());
  dist.resize(n, INF);

  for (int i = 0; i < m; ++i) {
    int u, v, w;
    std::cin >> u >> v >> w;
    u--, v--;
    g[u].push_back(Edge{v, w});
    k = std::max(k, w);
  }

  dist[0] = 0;
  bfs1k(0);
  for (int i = 1; i < n; ++i) {
    if (dist[i] == INF) {
      std::cout << "Impossible ";
    } else {
      std::cout << dist[i] << ' ';
    }
  }
  std::cout << std::endl;
}