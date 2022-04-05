#include <iostream>
#include <vector>
#include <queue>

#define int int64_t

struct Edge {
  int dst, w;
};

std::vector<int> dists;
std::vector<std::vector<Edge>> edges;
std::vector<int> negativeLoop;

int n, m, s;
int inf = INT64_MAX - 1;

void SPFA(int vertex) {
  dists.resize(n, inf);
  dists[vertex] = 0;
  std::deque<int> relaxated;
  std::deque<int> iteration;
  std::vector<char> onQueue(n, false);  // is vertex in queue now?
  relaxated.push_back(vertex);
  iteration.push_back(1);
  onQueue[vertex] = true;

  while (!relaxated.empty() && iteration.front() <= n) {
    int start = relaxated.front(); relaxated.pop_front();
    int i = iteration.front(); iteration.pop_front();
    onQueue[start] = false;

    for (const auto& edge: edges[start]) {
      if (dists[edge.dst] > dists[start] + edge.w) {
        // if relaxation happened in the last iteration -> the negative loop exists
        if (i == n) {
          dists[edge.dst] = inf + 1;
          negativeLoop.push_back(edge.dst);
          continue;
        }

        dists[edge.dst] = dists[start] + edge.w;
        if (!onQueue[edge.dst]) {
          onQueue[edge.dst] = true;

          // This is Small Label First optimization.
          // You can use queue instead of deque and always push to the back
          if (relaxated.empty() || dists[edge.dst] < dists[relaxated.front()]) {
            relaxated.push_front(edge.dst);
            iteration.push_front(i + 1);
          } else {
            relaxated.push_back(edge.dst);
            iteration.push_back(i + 1);
          }
        }
      }
    }
  }
}

void dfs(int vertex) {
  dists[vertex] = inf + 1;
  for (const Edge& e: edges[vertex]) {
    if (dists[e.dst] < inf) {
      dfs(e.dst);
    }
  }
}

// solution for https://codeforces.com/group/QmrArgR1Jp/contest/302599/problem/E
// Find the shortest distance from the first vertex to all others.
// The graph is oriented, weighted and can contain loops and multiple edges between a pair of vertices.
int32_t main() {
  std::cin >> n >> m >> s;
  s--;
  edges.resize(n);
  for (int i = 0; i < m; ++i) {
    int u, v, w;
    std::cin >> u >> v >> w;
    u--, v--;
    edges[u].push_back(Edge{v, w});
  }

  SPFA(s);

  for (int v: negativeLoop) {
    dfs(v);
  }

  for (int i = 0; i < n; ++i) {
    if (dists[i] == inf) {
      std::cout << "*" << '\n';  // the vertex is unreachable
    } else if (dists[i] == inf + 1) {
      std::cout << "-" << '\n';  // the vertex is reachable but there is no shortest path
    } else {
      std::cout << dists[i] << '\n';
    }
  }

  return 0;
}
