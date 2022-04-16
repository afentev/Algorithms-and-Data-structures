#include <iostream>
#include <vector>

using namespace std;

class TreeAncestor {
 public:
  TreeAncestor(int n, const vector<int>& p): n(n) {
    int lg = 32 - __builtin_clz(n - 1);
    liftings.resize(n, vector<int> (lg, -1));

    for (int i = 0; i < n; ++i) {
      liftings[i][0] = p[i];
    }

    for (int j = 1; j < lg; ++j) {
      for (int i = 0; i < n; ++i) {
        if (liftings[i][j - 1] >= 0) {
          liftings[i][j] = liftings[liftings[i][j - 1]][j - 1];
        } else {
          liftings[i][j] = -1;
        }
      }
    }
  }

  int getKthAncestor(int node, int k) {
    if (n <= k) {
      return -1;
    }
    while (k && node != -1) {
      int index = __builtin_ctz(k);
      node = liftings[node][index];
      k &= k - 1;
    }
    return node;
  }

  int n;
  vector<vector<int>> liftings;
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
