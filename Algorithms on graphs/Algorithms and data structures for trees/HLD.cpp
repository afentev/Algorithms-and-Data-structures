#include <iostream>
#include <vector>
#include <numeric>

#define int int64_t

struct SegmentTree {
  static inline int neutral = 2e18;
  int op(int arg1, int arg2) {
    return std::min(arg1, arg2);
  }

  SegmentTree() = default;

  SegmentTree(const std::vector<int>& a) {
    n = a.size();
    t.resize(n << 2, neutral);
    propagation.resize(n << 2, 0);
    propagateAdd.resize(n << 2, 0);
    propagateSet.resize(n << 2, 0);
    build(a, 1, 0, n - 1);
  }

  void build(const std::vector<int>& a, int i, int lSubSeg, int rSubSeg) {
    if (rSubSeg == lSubSeg == 1) {
      t[i] = a[lSubSeg];
    } else {
      int mid = (lSubSeg + rSubSeg) >> 1;
      build(a, i << 1, lSubSeg, mid);
      build(a, (i << 1) | 1, mid + 1, rSubSeg);
      t[i] = op(t[i << 1], t[(i << 1) | 1]);
    }
  }

  int query(int l, int r) {  // [l; r]
    return query(1, 0, n - 1, l, r);
  }

  int query(int i, int lSubSeg, int rSubSeg, int lQuery, int rQuery) {
    push(i, lSubSeg, rSubSeg);

    if (lQuery > rSubSeg || rQuery < lSubSeg) {
      return neutral;
    }
    if (lQuery <= lSubSeg && rSubSeg <= rQuery) {
      return t[i];
    }
    int mid = (lSubSeg + rSubSeg) >> 1;
    int left = query(i << 1, lSubSeg, mid, lQuery, rQuery);
    int right = query((i << 1) | 1, mid + 1, rSubSeg, lQuery, rQuery);

    return op(left, right);
  }

  void set(int pos, int newVal) {
    massSet(1, 0, n - 1, pos, pos, newVal);
  }

  void massAdd(int delta, int l, int r) {
    massAdd(1, 0, n - 1, l, r, delta);
  }

  void massAdd(int i, int lSubSeg, int rSubSeg, int lQuery, int rQuery, int delta) {
    push(i, lSubSeg, rSubSeg);

    if (lQuery > rSubSeg || rQuery < lSubSeg) {
      return;
    }
    int mid = (lSubSeg + rSubSeg) >> 1;
    if (lQuery <= lSubSeg && rSubSeg <= rQuery) {
      if (lSubSeg != rSubSeg) {
        if (propagation[i << 1] == 1) {
          push(i << 1, lSubSeg, mid);
          push((i << 1) | 1, mid + 1, rSubSeg);
        }
        propagation[(i << 1)] = propagation[(i << 1) | 1] = 2;
        propagateAdd[(i << 1)] += delta;
        propagateAdd[(i << 1) | 1] += delta;
      }
      t[i] += delta;
    } else {
      massAdd(i << 1, lSubSeg, mid, lQuery, rQuery, delta);
      massAdd((i << 1) | 1, mid + 1, rSubSeg, lQuery, rQuery, delta);
      t[i] = op(t[i << 1], t[(i << 1) | 1]);
    }
  }

  void massSet(int newVal, int l, int r) {
    massSet(1, 0, n - 1, l, r, newVal);
  }

  void massSet(int i, int lSubSeg, int rSubSeg, int lQuery, int rQuery, int newVal) {
    push(i, lSubSeg, rSubSeg);

    if (lQuery > rSubSeg || rQuery < lSubSeg) {
      return;
    }
    if (lQuery <= lSubSeg && rSubSeg <= rQuery) {
      if (lSubSeg != rSubSeg) {
        propagation[(i << 1)] = propagation[(i << 1) | 1] = 1;
        propagateSet[(i << 1)] = propagateSet[(i << 1) | 1] = newVal;
        propagateAdd[i << 1] = propagateAdd[(i << 1) | 1] = 0;
      }
      t[i] = newVal;
    } else {
      int mid = (lSubSeg + rSubSeg) >> 1;
      massSet(i << 1, lSubSeg, mid, lQuery, rQuery, newVal);
      massSet((i << 1) | 1, mid + 1, rSubSeg, lQuery, rQuery, newVal);
      t[i] = op(t[i << 1], t[(i << 1) | 1]);
    }
  }

  void push(int pos, int l, int r) {
    bool isLeaf = l == r;
    int mid = (l + r) >> 1;

    if (propagation[pos] == 0) {
      return;
    }
    if (propagation[pos] == 1) {
      t[pos] = propagateSet[pos];
      if (!isLeaf) {
        propagateSet[pos << 1] = propagateSet[(pos << 1) | 1] = propagateSet[pos];
        propagateAdd[pos << 1] = propagateAdd[(pos << 1) | 1] = propagateAdd[pos] = 0;
      }
    } else {
      t[pos] += propagateAdd[pos];
      if (!isLeaf) {
        if (propagation[pos << 1] == 1) {
          push(pos << 1, l, mid);
          push((pos << 1) | 1, mid + 1, r);
        }
        propagateAdd[pos << 1] += propagateAdd[pos];
        propagateAdd[(pos << 1) | 1] += propagateAdd[pos];
      }
      propagateAdd[pos] = 0;
    }
    if (!isLeaf) {
      propagation[pos << 1] = propagation[(pos << 1) | 1] = propagation[pos];
    }
    propagation[pos] = 0;
  }

  std::vector<int> t;
  std::vector<int> propagateSet;
  std::vector<int> propagateAdd;
  std::vector<char> propagation;  // 0 - no propagation, 1 - last set, 2 - last add
  int n = 0;
};

std::vector<std::vector<int>> graph;
std::vector<std::vector<int>> children;
std::vector<bool> used;

void rooting(int vertex) {
  used[vertex] = true;
  for (int u: graph[vertex]) {
    if (!used[u]) {
      children[vertex].push_back(u);
      rooting(u);
    }
  }
}

struct HLD {
  HLD(const std::vector<std::vector<int>>& g): g(g) {
    n = g.size();

    eulerTour.reserve(2 * n);
    heavy.resize(n, -1);
    pathHead.resize(n, -1);
    depth.resize(n, 0);
    values.resize(n, 0);
    parent.resize(n, -1);
    st = SegmentTree(values);

    depth[0] = 0;
    dfs(0);
    decompose(0, 0);
  }

  int dfs(int vertex) {
    int subtree = 1;
    int maxima = 0;
    for (int u: g[vertex]) {
      parent[u] = vertex;
      depth[u] = depth[vertex] + 1;
      int size = dfs(u);
      subtree += size;
      if (size > maxima) {
        maxima = size;
        heavy[vertex] = u;
      }
    }
    return subtree;
  }

  void decompose(int vertex, int head) {
    eulerTour[vertex] = pos++;
    pathHead[vertex] = head;
    if (heavy[vertex] != -1) {
      decompose(heavy[vertex], head);
    }
    for (int u: g[vertex]) {
      if (u != heavy[vertex]) {
        decompose(u, u);
      }
    }
  }

  void massAdd(int u, int v, int delta) {
    for (; pathHead[u] != pathHead[v]; v = parent[pathHead[v]]) {
      if (depth[pathHead[u]] > depth[pathHead[v]]) {
        std::swap(u, v);
      }
      st.massAdd(delta, eulerTour[pathHead[v]], eulerTour[v]);
    }
    if (depth[u] > depth[v]) {
      std::swap(u, v);
    }
    st.massAdd(delta, eulerTour[u], eulerTour[v]);
  }

  int get(int u) {
    return st.query(eulerTour[u], eulerTour[u]);
  }

  int pos = 0;
  int n;
  std::vector<std::vector<int>> g;
  std::vector<int> depth;
  std::vector<int> heavy;
  std::vector<int> eulerTour;
  std::vector<int> pathHead;
  std::vector<int> values;
  std::vector<int> parent;
  SegmentTree st;
};


// solution for https://codeforces.com/group/QmrArgR1Jp/contest/279285/problem/G
int32_t main() {
  std::cin.tie(nullptr);
  std::ios_base::sync_with_stdio(false);

  int n;
  std::cin >> n;

  graph.resize(n);
  children.resize(n);
  used.resize(n, false);
  for (int i = 0; i < n - 1; ++i) {
    int u, v;
    std::cin >> u >> v;
    --u, --v;
    graph[u].push_back(v);
    graph[v].push_back(u);
  }
  rooting(0);

  HLD hld(children);

  int q;
  std::cin >> q;
  while (q--) {
    char command;
    std::cin >> command;
    if (command == '+') {
      int u, v, delta;
      std::cin >> u >> v >> delta;
      --u, --v;
      hld.massAdd(u, v, delta);
    } else {
      int v;
      std::cin >> v;
      --v;
      std::cout << hld.get(v) << '\n';
    }
  }
  return 0;
}