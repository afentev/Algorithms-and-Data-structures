#include <iostream>
#include <queue>
#include <vector>
#include <map>

const int INF = 1e9;
int k = -INF;
int n, begin, end;

std::vector<std::map<int, int>> g;
std::vector<int> dist;


void bfs1k(int vertex) {
  std::map<int, std::queue<int>>q;
  q[0].push(vertex);
  while (!q.empty()) {
    auto ptr = q.begin();
    auto start = *q.begin();
    q.erase(ptr);
    while (!start.second.empty()) {
      int u = start.second.front();
      start.second.pop();

      for (auto edge: g[u]) {
        if (dist[u] + edge.second < dist[edge.first]) {
          dist[edge.first] = dist[u] + edge.second;
          q[dist[edge.first] % (k + 1)].push(edge.first);
        }
      }
    }
  }
}


/*
 * solution for https://informatics.msk.ru/mod/statements/view.php?id=193&chapterid=5#1
 */
int main() {
  std::cin >> n >> begin >> end;
  begin--, end--;

  g.resize(n, std::map<int, int>());
  dist.resize(n, INF);

  for (int i = 0; i < n; ++i) {
    for (int j = 0; j < n; ++j) {
      int x;
      std::cin >> x;
      if (x == 0 || x == -1) {
        continue;
      }
      g[i][j] = x;
      k = std::max(k, x);
    }
  }

  dist[begin] = 0;
  bfs1k(begin);
  if (dist[end] == INF) {
    std::cout << -1;
  } else {
    std::cout << dist[end];
  }
  std::cout << std::endl;
}