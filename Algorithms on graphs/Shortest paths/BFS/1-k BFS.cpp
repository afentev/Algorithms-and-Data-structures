#include <iostream>
#include <queue>
#include <vector>
#include <map>

const int INF = 1e9;
int k = -INF;
int n, m;

std::vector<std::map<int, int>> g;
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
      for (auto v: g[u]) {
        if (dist[u] + v.second < dist[v.first]) {
          dist[v.first] = dist[u] + v.second;
          ar[(queue + v.second) % (k + 1)].push(v.first);
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
  g.resize(n, std::map<int, int>());
  dist.resize(n, INF);

  for (int i = 0; i < m; ++i) {
    int u, v, w;
    std::cin >> u >> v >> w;
    u--, v--;
    g[u][v] = w;
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