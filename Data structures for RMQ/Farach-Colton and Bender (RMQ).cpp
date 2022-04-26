#include <iostream>
#include <vector>
#include <stack>

using namespace std;


struct SparseTable {
  SparseTable() = default;

  SparseTable(const vector<pair<int, int>>& a) {
    int n = a.size();
    int pw = 32 - __builtin_clz(n);
    table.resize(pw, vector<pair<int, int>> (n, {0, 0}));
    table[0] = a;
    for (int i = 0; i + 1 < pw; ++i) {
      int powered = 1 << i;
      for (int ind = 0; ind + powered < n; ++ind) {
        table[i + 1][ind] = min(table[i][ind], table[i][ind + powered]);
      }
    }
  }

  pair<int, int> query(int l, int r) {  // [l; r)
    if (l > r) {
      swap(l, r);
    }
    int power = 31 - __builtin_clz(r - l);
    return min(table[power][l], table[power][r - (1u << power)]);
  }

  vector<vector<pair<int, int>>> table;
};


struct FCB_LCA {
  FCB_LCA (const vector<pair<int, int>>& v): eulerTour(v), B(max(1, (32 - __builtin_clz(v.size() / 2 - 1)) / 2)) {
    vector<pair<int, int>> array(v.size() / B + 1, {INT32_MAX, INT32_MAX});
    for (int i = 0; i < v.size(); ++i) {
      array[i / B] = min(array[i / B], v[i]);
    }
    st = SparseTable(array);
    precalc.resize(1u << (B - 1));
    masks.resize(array.size(), -1);
    for (int i = 0; i < v.size(); i += B) {
      uint32_t mask = 0;
      for (int j = i + 1; j - i < B && j < v.size(); ++j) {
        if (v[j].first == v[j - 1].first + 1) {
          mask |= (1u << (B - (j - i) - 1));
        }
      }
      masks[i / B] = mask;
      if (precalc[mask].empty()) {
        precalc[mask].resize(B, vector<int>(B, INT32_MAX));
        for (int start = 0; start < B; ++start) {
          pair<int, int> minima = v[i + start];
          int minimaIndex = 0;
          for (int end = start; end < B; ++end) {
            if (v[i + end] < minima) {
              minima = v[i + end];
              minimaIndex = end - start;
            }
            precalc[mask][start][end] = minimaIndex;
          }
        }
      }
    }
  }

  FCB_LCA& operator= (const FCB_LCA& other) {
    st = other.st;
    eulerTour = other.eulerTour;
    masks = other.masks;
    precalc = other.precalc;
    return *this;
  };

  int query(int l, int r) {
    int lBlock = l / B;
    int rBlock = r / B;
    if (lBlock == rBlock) {
      return eulerTour[l + precalc[masks[lBlock]][l % B][r % B]].second;
    }
    pair<int, int> prefix = eulerTour[l + precalc[masks[lBlock]][l % B][B - 1]];
    pair<int, int> infix = {INT32_MAX, INT32_MAX};
    if (lBlock + 1 < rBlock) {
      infix = st.query(lBlock + 1, rBlock);
    }
    pair<int, int> suffix = eulerTour[r - r % B + precalc[masks[rBlock]][0][r % B]];
    return min({prefix, infix, suffix}).second;
  }

  const int B;
  SparseTable st;
  vector<pair<int, int>> eulerTour;
  vector<uint32_t> masks;
  vector<vector<vector<int>>> precalc;  // mask -> query L -> query R
};


struct FCB_RMQ {
  FCB_RMQ(const vector<int>& v): eulerTour(2 * v.size(), {0, 0}), fcb(eulerTour) {
    array = v;
    parents.resize(array.size(), 0);
    stack<int> s;
    s.push(0);
    int root = 0;
    vector<int> left(array.size(), -1);
    vector<int> right(array.size(), -1);

    for (int i = 1; i < array.size(); ++i) {
      int last = -1;
      while (!s.empty() && array[s.top()] >= array[i]) {
        last = s.top();
        s.pop();
      }
      left[i] = last;
      if (!s.empty()) {
        right[s.top()] = i;
      } else {
        root = i;
      }
      s.push(i);
    }

    children.resize(array.size());
    for (int i = 0; i < array.size(); ++i) {
      if (left[i] != -1) {
        children[i].push_back(left[i]);
      }
      if (right[i] != -1) {
        children[i].push_back(right[i]);
      }
    }

    d.resize(array.size(), 0);
    indexes.resize(array.size(), 0);
    dfs(root, 0);
    fcb = FCB_LCA(eulerTour);
  }

  void dfs(int vertex, int depth) {
    d[vertex] = depth;
    eulerTour[pos] = {depth, vertex};
    indexes[vertex] = pos;
    ++pos;
    for (int child: children[vertex]) {
      dfs(child, depth + 1);
      eulerTour[pos] = {depth, vertex};
      ++pos;
    }
  }

  int query(int l, int r) {  // min on [l; r]
    int left = indexes[l];
    int right = indexes[r];
    if (left > right) {
      swap(left, right);
    }
    return array[fcb.query(left, right)];
  }

  vector<int> array;
  vector<int> parents;
  vector<vector<int>> children;
  vector<int> d;  // dists
  vector<pair<int, int>> eulerTour;
  vector<int> indexes;
  int pos = 0;

  FCB_LCA fcb;
};

// solution for https://codeforces.com/group/QmrArgR1Jp/contest/279285/problem/B
int32_t main() {
  freopen("sparse.in","r",stdin);
  freopen("sparse.out","w",stdout);

  cin.tie(nullptr);
  ios_base::sync_with_stdio(false);

  int n, m, f;
  std::cin >> n >> m >> f;
  std::vector<int> a(n, 0);
  a[0] = f;
  for (int i = 1; i < n; ++i) {
    a[i] = (23 * a[i - 1] + 21563) % 16714589;
  }

  FCB_RMQ x(a);

  int u, v;
  std::cin >> u >> v;
  int ans;
  for (int i = 1; i <= m; ++i) {
    ans = x.query(std::min(u, v) - 1, std::max(v, u) - 1);
    if (i != m) {
      u = ((17 * u + 751 + ans + 2 * i) % n) + 1;
      v = ((13 * v + 593 + ans + 5 * i) % n) + 1;
    }
  }

  std::cout << u << ' ' << v << ' ' << ans << '\n';

  return 0;
}
