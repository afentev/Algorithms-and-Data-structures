#include <iostream>
#include <vector>
#include <tuple>
#include <set>
#include <cmath>

#define int int64_t

struct Edge {
  int src, dst, w;

  bool operator< (const Edge& other) const {
    return std::make_tuple(src, dst, w) < std::make_tuple(other.src, other.dst, other.w);
  }
};

std::vector<int> dists;
std::set<Edge> edges;
std::vector<int> negativeLoop;

int n, m, s;
int inf = INT64_MAX - 1;

void FordBellman(int vertex) {
  dists.resize(n, inf);
  dists[vertex] = 0;
  for (int i = 1; i <= n; ++i) {
    for (const auto& edge: edges) {
      int old = dists[edge.dst];

      if (dists[edge.src] >= inf) {
        continue;
      }
      dists[edge.dst] = std::min(dists[edge.dst], dists[edge.src] + edge.w);

      // if relaxation happened in the last iteration -> the negative loop exists
      if (old != dists[edge.dst] && i == n) {
        dists[edge.dst] = inf + 1;
        negativeLoop.push_back(edge.dst);
      }
    }
  }
}

void dfs(int vertex) {
  dists[vertex] = inf + 1;
  auto start = edges.lower_bound({vertex, 0, INT64_MIN});
  while (start != edges.end() && start->src == vertex) {
    if (dists[start->dst] < inf) {
      dfs(start->dst);
    }
    start = std::next(start);
  }
}

// solution for https://codeforces.com/group/QmrArgR1Jp/contest/302599/problem/E
// Find the shortest distance from the first vertex to all others.
// The graph is oriented, weighted and can contain loops and multiple edges between a pair of vertices.
int32_t main() {
  std::cin >> n >> m >> s;
  s--;
  for (int i = 0; i < m; ++i) {
    int u, v, w;
    std::cin >> u >> v >> w;
    u--, v--;
    edges.insert({u, v, w});
  }

  FordBellman(s);

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
