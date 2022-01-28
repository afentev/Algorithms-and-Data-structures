#include <iostream>
#include <vector>
#include <unordered_set>


struct Edge {
  int src, dst;
};


int n, m;
std::vector<std::unordered_set<int>> g;
std::vector<std::unordered_set<int>> hv;
std::vector<Edge> edges;
std::vector<Edge> directedEdges;
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
    hv[src].insert(dst);
  }

  for (int u = 0; u < n; ++u) {
    int cnt = 0;
    for (int v: hv[u]) {
      for (int w: hv[u]) {
        if (g[v].contains(w)) {
          trinagles++;
        }
      }
    }
  }
}

int main() {
  std::cin >> n >> m;
  g.resize(n, std::unordered_set<int>());
  hv.resize(n, std::unordered_set<int>());
  edges.reserve(m);

  for (int i = 0; i < m; ++i) {  // unoriented graph
    int u, v;
    std::cin >> u >> v;
    u--, v--;
    edges.push_back({u, v});
    g[u].insert(v);
    g[v].insert(u);
  }

  countTriangles();
  trinagles /= 2;

  std::cout << trinagles << std::endl;
}