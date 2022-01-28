#include <iostream>
#include <vector>
#include <utility>
#include <set>
#include <map>

std::vector<std::set<int>> g;
std::vector<int> sort;
std::vector<int> colors;
bool cycle = false;

int n, m;
int order = 1;

void dfs(int vertex) {
  colors[vertex] = 1;
  for (int u: g[vertex]) {
    if (colors[u] == 0) {
      dfs(u);
    } else if (colors[u] == 1) {
      cycle = true;
    }
  }
  sort.push_back(vertex);
  colors[vertex] = 2;
}

void topsort() {
  sort.reserve(n);
  colors.resize(n, 0);
  for (int i = 0; i < n; ++i) {
    if (colors[i] == 0) {
      dfs(i);
    }
  }
}

// solution for https://codeforces.com/group/QmrArgR1Jp/contest/298379/problem/A
int main() {
  std::cin >> n >> m;
  g.resize(n);
  for (int i = 0; i < m; ++i) {
    int u, v;
    std::cin >> u >> v;
    u--, v--;
    g[u].insert(v);
  }

  topsort();

  if (cycle) {
    std::cout << -1 << std::endl;  // graph contains a cycle => topological sort is impossible
    return 0;
  }
  std::reverse(sort.begin(), sort.end());
  for (int i = 0; i < n; ++i) {
    std::cout << sort[i] + 1 << ' ';
  }
  std::cout << std::endl;

  return 0;
}
