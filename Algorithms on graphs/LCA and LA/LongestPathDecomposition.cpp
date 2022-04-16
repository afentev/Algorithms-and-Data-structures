#include <iostream>
#include <vector>

using namespace std;


class TreeAncestor {
 public:
  TreeAncestor(int n, vector<int>& p) {
    parents = p;

    children.resize(n, vector<int>());
    for (int i = 1; i < n; ++i) {
      children[p[i]].push_back(i);
    }

    matching.resize(n, {0, 0});
    createPaths(0, 0);
  }

  int getKthAncestor(int node, int k) {
    while (k > 0) {
      auto index = matching[node];
      const auto& path = longestPaths[index.first];
      if (index.second + k < path.size()) {
        return path[index.second + k];
      }
      if (parents[path.back()] == -1) {
        return -1;
      }
      node = parents[path.back()];
      k -= path.size() - index.second;
    }
    return node;
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

  vector<int> parents;
  vector<vector<int>> children;
  vector<vector<int>> longestPaths;
  vector<pair<int, int>> matching;
};


// solution for https://leetcode.com/problems/kth-ancestor-of-a-tree-node/
int main() {
  auto x = vector<int>{-1,0,0,0,3};
  TreeAncestor* treeAncestor = new TreeAncestor(5, x);
  cout << treeAncestor->getKthAncestor(1, 5) << '\n';
  cout << treeAncestor->getKthAncestor(3, 2) << '\n';
  cout << treeAncestor->getKthAncestor(0, 1) << '\n';
  cout << treeAncestor->getKthAncestor(3, 1) << '\n';
  cout << treeAncestor->getKthAncestor(3, 5) << '\n';
  delete treeAncestor;
}
