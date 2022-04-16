#include <iostream>
#include <vector>
#include <cmath>
#include <queue>

using namespace std;


class TreeAncestor {
 public:
  TreeAncestor(int n, vector<int>& p) {
    d.resize(n, 0);
    d[0] = 0;

    vector<vector<int>> children(n, vector<int>());
    for (int i = 1; i < n; ++i) {
      children[p[i]].push_back(i);
    }
    queue<int> q;
    q.push(0);
    while (!q.empty()) {
      int u = q.front(); q.pop();
      for (int child: children[u]) {
        q.push(child);
        d[child] = d[u] + 1;
      }
    }

    int lg = ceil(log2(n));
    dp.resize(n, vector<int> (lg, 0));  // 0-th vertex is root

    // preprocess
    for (int i = 0; i < n; ++i) {
      dp[i][0] = p[i];
    }

    for (int j = 1; j < lg; ++j) {
      for (int i = 0; i < n; ++i) {
        if (dp[i][j - 1] != -1) {
          dp[i][j] = dp[dp[i][j - 1]][j - 1];
        } else {
          dp[i][j] = -1;
        }
      }
    }
  }

  int getKthAncestor(int node, int k) {
    if (k > d[node]) {
      return -1;
    }
    while (k) {
      int index = __builtin_ctz(k);
      node = dp[node][index];
      k &= k - 1;
    }
    return node;
  }

  vector<int> d;
  vector<vector<int>> dp;
};


// solution for https://leetcode.com/problems/kth-ancestor-of-a-tree-node/
int main() {
  auto x = vector<int>{-1,2,3,0};
  TreeAncestor* treeAncestor = new TreeAncestor(4, x);
  cout << treeAncestor->getKthAncestor(2, 3) << '\n';
  cout << treeAncestor->getKthAncestor(2, 2) << '\n';
  cout << treeAncestor->getKthAncestor(2, 1) << '\n';
  delete treeAncestor;
}
