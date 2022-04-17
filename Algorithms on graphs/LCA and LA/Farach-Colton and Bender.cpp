#include <iostream>
#include <vector>

using namespace std;

int n;
vector<int> p;
vector<int> d;  // dists
vector<vector<int>> children;
vector<pair<int, int>> eulerTour;
vector<int> indexes;
int pos = 0;


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

struct FCB {
  FCB (const vector<pair<int, int>>& v): eulerTour(v), B((32 - __builtin_clz(n - 1)) / 2) {
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

void dfs(int vertex) {
  eulerTour[pos] = {d[vertex], vertex};
  indexes[vertex] = pos;
  ++pos;
  for (int child: children[vertex]) {
    dfs(child);
    eulerTour[pos] = {d[vertex], vertex};
    ++pos;
  }
}

// solution for https://codeforces.com/group/QmrArgR1Jp/contest/279285/problem/B
int main() {
  cin.tie(nullptr);
  ios_base::sync_with_stdio(false);

  cin >> n;
  p.resize(n, 0);
  d.resize(n, 0);

  children.resize(n, vector<int>());

  // read parents
  p[0] = 0;
  d[0] = 0;
  for (int i = 1; i < n; ++i) {
    cin >> p[i];
    --p[i];
    children[p[i]].push_back(i);
    d[i] = d[p[i]] + 1;
  }

  eulerTour.resize(2 * n, {0, 0});
  indexes.resize(n, 0);
  dfs(0);
  FCB fcb(eulerTour);

  int m;  // number of queries
  cin >> m;
  for (int i = 0; i < m; ++i) {
    int u, v;
    cin >> u >> v;
    --u, --v;
    int left = indexes[u];
    int right = indexes[v];
    if (left > right) {
      swap(left, right);
    }
    cout << fcb.query(left, right) + 1 << '\n';
  }

  return 0;
}
