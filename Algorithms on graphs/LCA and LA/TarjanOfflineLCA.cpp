#include <iostream>
#include <set>
#include <vector>
#include <cmath>
#include <unordered_map>
#include <numeric>

using namespace std;


struct DSU {
  DSU(size_t size_ = 0) {
    init(size_);
  }

  void init(size_t size_) {
    parent.resize(size_, 0);
    iota(parent.begin(), parent.end(), 0);
    size.resize(size_, 1);
    ancestor.resize(size_, -1);
  }

  void make_set(int v) {
    parent[v] = v;
    size[v] = 1;
  }

  void union_sets(int v, int u, int newAncestor) {
    union_roots(find_set(v), find_set(u), newAncestor);
  }

  void union_roots(int v, int u, int newAncestor) {
    if (v != u) {
      if (size[v] > size[u]) {
        swap(v, u);
      }
      parent[v] = u;
      size[u] += size[v];
      ancestor[u] = newAncestor;
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
  vector<int> ancestor;
};


struct hash_pair {
  template <class T1, class T2>
  size_t operator()(const pair<T1, T2>& p) const{
    auto hash1 = hash<T1>{}(p.first);
    auto hash2 = hash<T2>{}(p.second);
    return hash1 ^ hash2;
  }
};

int n;
vector<int> p;
vector<int> d;  // dists

DSU dsu;
vector<vector<int>> children;
vector<vector<int>> queries;
unordered_map<pair<int, int>, int, hash_pair> ans;
vector<char> visited;

void dfs(int v) {
  for (int child: children[v]) {
    if (!visited[child]) {
      dfs(child);
      dsu.union_sets(v, child, v);
    }
  }
  for (int u: queries[v]) {
    if (visited[u]) {
      ans[{u + 1, v + 1}] = ans[{v + 1, u + 1}] = dsu.ancestor[dsu.find_set(u)] + 1;
    }
  }
  visited[v] = true;
}

// solution for https://codeforces.com/group/QmrArgR1Jp/contest/279285/problem/B
int main() {
  cin.tie(nullptr);
  cout.tie(nullptr);
  ios_base::sync_with_stdio(false);

  cin >> n;
  p.resize(n, 0);
  d.resize(n, 0);

  // read parents
  p[0] = 0;
  d[0] = 0;
  children.resize(n);
  for (int i = 1; i < n; ++i) {
    cin >> p[i];
    p[i]--;
    children[p[i]].push_back(i);
    d[i] = d[p[i]] + 1;
  }

  dsu.init(n);

  int m;  // number of queries
  cin >> m;
  queries.resize(n);
  vector<pair<int, int>> requests;
  requests.reserve(m);
  for (int i = 0; i < m; ++i) {
    int u, v;
    cin >> u >> v;
    requests.emplace_back(u, v);
    u--, v--;
    queries[u].push_back(v);
    queries[v].push_back(u);
  }
  visited.resize(n, false);
  ans.reserve(m);
  ans.max_load_factor(0.5);
  dfs(0);
  for (const auto& q: requests) {
    cout << ans[q] << '\n';
  }
  return 0;
}