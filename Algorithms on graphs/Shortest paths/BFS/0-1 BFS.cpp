#include <iostream>
#include <deque>
#include <vector>
#include <utility>
#include <map>

const int INF = 1e9;

std::vector<std::map<int, bool>> g;
std::vector<int> dist;

void bfs01(int vertex) {
  std::deque<std::pair<int, int>> q {{vertex, 0}};
  while (!q.empty()) {
    auto v = q.front(); q.pop_front();
    if (v.second >= dist[v.first]) {
      continue;  // we already know the better path
    }
    dist[v.first] = v.second;

    for (auto dst: g[v.first]) {
      if (!dst.second) {  // weight == 0
        q.push_front({dst.first, v.second});
      } else {
        q.push_back({dst.first, v.second + dst.second});
      }
    }
  }
}

int main() {
  int n, start, finish;
  std::cin >> n >> start >> finish;
  start--, finish--;

  g.resize(n, std::map<int, bool>());
  dist.resize(n, INF);

  for (int i = 0; i < n; ++i) {
    int cnt;
    std::cin >> cnt;
    for (int j = 0; j < cnt; ++j) {
      int dst;
      std::cin >> dst;
      dst--;
      g[i][dst] = (j != 0);
    }
  }

  bfs01(start);
  if (dist[finish] != INF) {
    std::cout << dist[finish] << std::endl;
  } else {
    std::cout << -1 << std::endl;
  }
}