#include <iostream>
#include <vector>

int n;
std::vector<int> p;
std::vector<int> d;  // dists
std::vector<std::vector<int>> children;
std::vector<std::pair<int, int>> eulerTour;
std::vector<int> indexes;


struct SparseTable {
  SparseTable(const std::vector<std::pair<int, int>>& a) {
    int n = a.size();
    int pw = 32 - __builtin_clz(n);
    table.resize(pw, std::vector<std::pair<int, int>> (n, {0, 0}));
    table[0] = a;
    for (int i = 0; i + 1 < pw; ++i) {
      int powered = 1 << i;
      for (int ind = 0; ind + powered < n; ++ind) {
        table[i + 1][ind] = std::min(table[i][ind], table[i][ind + powered]);
      }
    }
  }

  int query(int l, int r) {  // [l; r)
    if (l > r) {
      std::swap(l, r);
    }
    int power = 31 - __builtin_clz(r - l);
    return std::min(table[power][l], table[power][r - (1u << power)]).second;
  }

  std::vector<std::vector<std::pair<int, int>>> table;
};

void dfs(int vertex) {
  indexes[vertex] = eulerTour.size();
  eulerTour.emplace_back(d[vertex], vertex);
  for (int child: children[vertex]) {
    dfs(child);
    eulerTour.emplace_back(d[vertex], vertex);
  }
}


// solution for https://codeforces.com/group/QmrArgR1Jp/contest/279285/problem/B
int main() {
  std::cin.tie(nullptr);
  std::ios_base::sync_with_stdio(false);

  std::cin >> n;
  p.resize(n, 0);
  d.resize(n, 0);

  children.resize(n, std::vector<int>());

  // read parents
  p[0] = 0;
  d[0] = 0;
  for (int i = 1; i < n; ++i) {
    std::cin >> p[i];
    --p[i];
    children[p[i]].push_back(i);
    d[i] = d[p[i]] + 1;
  }

  eulerTour.reserve(2 * n);
  indexes.resize(n, 0);
  dfs(0);
  SparseTable st(eulerTour);

  int m;  // number of queries
  std::cin >> m;
  for (int i = 0; i < m; ++i) {
    int u, v;
    std::cin >> u >> v;
    --u, --v;
    std::cout << st.query(indexes[u], indexes[v]) + 1 << '\n';
  }

  return 0;
}