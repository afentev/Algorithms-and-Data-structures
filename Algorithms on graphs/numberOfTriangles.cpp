#include <iostream>
#include <vector>
#include <unordered_set>


struct Edge {
  int src, dst;
};


std::vector<std::unordered_set<int>> g;
std::vector<Edge> edges;
int trinagles = 0;

// complexity is O(E√E)
void countTriangles() {
  for (const Edge& e: edges) {
    int src, dst;
    if (g[e.src].size() < g[e.dst].size()) {
      src = e.src;
      dst = e.dst;
    } else {
      src = e.dst;
      dst = e.src;
    }
    for (int u: g[src]) {  // O(√E) vertexes here because src has <= neighbors than dst
      if (g[u].contains(dst)) {
        ++trinagles;
      }
    }
  }
}

int main() {
  int n, m;
  std::cin >> n >> m;
  g.resize(n, std::unordered_set<int>());
  edges.reserve(2 * m);

  for (int i = 0; i < m; ++i) {  // unoriented graph
    int u, v;
    std::cin >> u >> v;
    u--, v--;
    edges.push_back({u, v});
    edges.push_back({v, u});
    g[u].insert(v);
    g[v].insert(u);
  }

  countTriangles();
  trinagles /= 6;

  std::cout << trinagles << std::endl;
}