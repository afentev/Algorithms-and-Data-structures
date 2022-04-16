#include <iostream>
#include <vector>
#include <queue>
#include <cmath>

using namespace std;


class TreeAncestor {
 public:
  TreeAncestor(int n, vector<int>& p) {
    this->n = n;
    parents = p;

    children.resize(n, vector<int>());
    for (int i = 1; i < n; ++i) {
      children[p[i]].push_back(i);
    }

    matching.resize(n, {0, 0});
    createPaths(0, 0);
    createLadders();
    createBinLiftings();
  }

  int getKthAncestor(int node, int k) {
    if (k > d[node]) {
      return -1;
    }
    if (k == 0) {
      return node;
    }
    int power = 31 - __builtin_clz(k);
    k &= ~(1u << power);
    node = liftings[node][power];
    auto index = matching[node];
    return longestPaths[index.first][index.second + k];
  }

  void createBinLiftings() {
    d.resize(n, 0);
    d[0] = 0;

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
    liftings.resize(n, vector<int> (lg, 0));

    for (int i = 0; i < n; ++i) {
      liftings[i][0] = parents[i];
    }

    for (int j = 1; j < lg; ++j) {
      for (int i = 0; i < n; ++i) {
        if (liftings[i][j - 1] != -1) {
          liftings[i][j] = liftings[liftings[i][j - 1]][j - 1];
        } else {
          liftings[i][j] = -1;
        }
      }
    }
  }

  int createPaths(int vertex, int depth) {
    int maxima = depth;
    int continuation;
    for (int child: children[vertex]) {
      int curDepth = createPaths(child, depth + 1);
      if (curDepth > maxima) {
        maxima = curDepth;
        continuation = child;
      }
    }
    if (maxima != depth) {
      int index = matching[continuation].first;
      longestPaths[index].push_back(vertex);
      matching[vertex] = {index, longestPaths[index].size() - 1};
    } else {
      longestPaths.push_back(vector<int>{vertex});
      matching[vertex] = {longestPaths.size() - 1, 0};
    }
    return maxima;
  }

  void createLadders() {
    for (auto& path : longestPaths) {
      int size = longestPaths.size();
      for (int j = 0; j < size && parents[path.back()] != -1; ++j) {
        path.push_back(parents[path.back()]);
      }
    }
  }

  int n;
  vector<int> d;
  vector<vector<int>> liftings;
  vector<int> parents;
  vector<vector<int>> children;
  vector<vector<int>> longestPaths;
  vector<pair<int, int>> matching;
};


// solution for https://leetcode.com/problems/kth-ancestor-of-a-tree-node/
int main() {
  auto x = vector<int>{-1, 0, 0, 1, 1, 2, 2};
  TreeAncestor* treeAncestor = new TreeAncestor(7, x);
  cout << treeAncestor->getKthAncestor(3, 1) << '\n';
  cout << treeAncestor->getKthAncestor(5, 2) << '\n';
  cout << treeAncestor->getKthAncestor(6, 3) << '\n';
  delete treeAncestor;
}
