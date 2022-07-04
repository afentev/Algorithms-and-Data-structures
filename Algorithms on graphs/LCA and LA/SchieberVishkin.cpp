#include <iostream>
#include <vector>

using namespace std;

vector<uint32_t> p;
vector<uint32_t> level;  // dists
vector<vector<uint32_t>> children;


struct SchieberVishkin {
  SchieberVishkin(const vector<vector<uint32_t>>& t): tree(t) {
    uint32_t n = t.size();
    timer = 1;
    inlabel.resize(n, 0);
    ascendant.resize(n, 0);
    preOrder.resize(n, 0);
    head.resize(n + 1, 0);
    dfs(0);
    setAscendants(0, 0);
  }

  // subtree size is a return value
  uint32_t dfs(uint32_t vertex) {
    preOrder[vertex] = timer++;
    uint32_t size = 0;
    for (uint32_t child: children[vertex]) {
      size += dfs(child);
    }
    uint32_t i = leadingZeroMask((preOrder[vertex] - 1) ^ (preOrder[vertex] + size - 1));
    uint32_t newInlabel = (preOrder[vertex] + size) & ~(i - 1);
    inlabel[vertex] = newInlabel;
    head[inlabel[vertex]] = vertex;
    return size + 1;
  }

  void setAscendants(uint32_t vertex, uint32_t mask) {
    mask |= trailingZeroMask(inlabel[vertex]);
    ascendant[vertex] = mask;
    for (uint32_t child: children[vertex]) {
      setAscendants(child, mask);
    }
  }

  uint32_t lca(uint32_t u, uint32_t v) {
    if (inlabel[u] == inlabel[v]) {
      if (level[u] < level[v]) {
        return u;
      }
      return v;
    }
    uint32_t i = leadingZeroMask(inlabel[u] ^ inlabel[v]);
    uint32_t path = trailingZeroMask(ascendant[u] & ascendant[v] & ~(i - 1));

    if (trailingZeroMask(inlabel[u]) != path) {
      uint32_t w = leadingZeroMask(ascendant[u] & (path - 1));
      u = p[head[(inlabel[u] & (~w + 1)) | w]];
    }
    if (trailingZeroMask(inlabel[v]) != path) {
      uint32_t w = leadingZeroMask(ascendant[v] & (path - 1));
      v = p[head[(inlabel[v] & (~w + 1)) | w]];
    }
    if (preOrder[u] < preOrder[v]) {
      return u;
    }
    return v;
  }

  static uint32_t leadingZeroMask(uint32_t x) {
    if (x <= 1) {
      return 1;
    }
    return 1u << (31 - __builtin_clz(x));
  }

  static uint32_t trailingZeroMask(uint32_t x) {
    return 1u << __builtin_ctz(x);
  }

  uint32_t timer;
  vector<vector<uint32_t>> tree;
  vector<uint32_t> inlabel;
  vector<uint32_t> ascendant;
  vector<uint32_t> head;
  vector<uint32_t> preOrder;
};


// solution for https://codeforces.com/group/QmrArgR1Jp/contest/279285/problem/B
int main() {
  cin.tie(nullptr);
  ios_base::sync_with_stdio(false);

  int n;
  cin >> n;
  p.resize(n, 0);
  level.resize(n, 0);

  children.resize(n, vector<uint32_t>());

  // read parents
  p[0] = 0;
  level[0] = 0;
  for (int i = 1; i < n; ++i) {
    cin >> p[i];
    --p[i];
    children[p[i]].push_back(i);
    level[i] = level[p[i]] + 1;
  }
  SchieberVishkin sv(children);

  int m;  // number of queries
  cin >> m;
  for (int i = 0; i < m; ++i) {
    int u, v;
    cin >> u >> v;
    --u, --v;
    cout << sv.lca(u, v) + 1 << endl;
  }

  return 0;
}