#include <iostream>
#include <vector>
#include <numeric>

using namespace std;

// copied from "../../Other Data Structures/DSU.cpp"
struct DSU {
  DSU(size_t size_ = 0) {
    init(size_);
  }

  void init(size_t size_) {
    parent.resize(size_, 0);
    iota(parent.begin(), parent.end(), 0);
    size.resize(size_, 1);
  }

  void make_set(int v) {
    parent[v] = v;
    size[v] = 1;
  }

  void union_sets(int v, int u) {
    union_roots(find_set(v), find_set(u));
  }

  void union_roots(int v, int u) {
    if (v != u) {
      if (size[v] > size[u]) {
        swap(v, u);
      }
      parent[v] = u;
      size[v] += size[u];
    }
  }

  int find_set(int v) {
    if (v == parent[v]) {
      return v;
    }
    return parent[v] = find_set(parent[v]);
  }

  vector<int> parent;
  vector<int> size;
};

int n, m;
int64_t weight = 0;

struct Edge {
  int src, dst, weight;
  bool operator< (const Edge& other) const {
    return weight < other.weight;
  }
};

std::vector<Edge> edges;

void kruskal() {
  std::sort(edges.begin(), edges.end());

  DSU dsu(n);
  for (int i = 0; i < m; ++i) {
    int src = dsu.find_set(edges[i].src);
    int dst = dsu.find_set(edges[i].dst);
    if (src != dst) {
      dsu.union_roots(src, dst);
      weight += edges[i].weight;
    }
  }
}

// solution for https://codeforces.com/group/QmrArgR1Jp/contest/298379/problem/J
int main() {
  cin.tie(nullptr);
  ios_base::sync_with_stdio(false);

  cin >> n >> m;
  edges.reserve(m);
  for (int i = 0; i < m; ++i) {
    int u, v, w;
    cin >> u >> v >> w;
    u--, v--;
    edges.push_back(Edge{u, v, w});
  }

  kruskal();

  cout << weight << '\n';
}