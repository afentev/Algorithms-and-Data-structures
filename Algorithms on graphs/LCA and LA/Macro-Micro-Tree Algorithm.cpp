#include <iostream>
#include <vector>
#include <cmath>
#include <stack>

using namespace std;


class TreeAncestor {
 public:
  TreeAncestor(int n, vector<int>& p): B((32 - __builtin_clz(n - 1)) / 4) {
    this->n = n;
    parents = p;

    children.resize(n, vector<int>());
    for (int i = 1; i < n; ++i) {
      children[p[i]].push_back(i);
    }

    macromicro();
    matching.resize(n, {0, 0});
    createPaths(0, 0);
    createLadders();
  }

  int getKthAncestor(int node, int k) {
    if (k > d[node]) {
      return -1;
    }
    if (isPruned[node]) {
      if (microDepth[node] >= k) {
        return precalc[microTreeType[node]][vertexEuler[node]][k];
      } else {
        k -= microDepth[node] + 1;
        node = ptr2child[microRoot[node]];
      }
    }
    k += d[ptr2child[node]] - d[node];
    node = ptr2child[node];
    if (k == 0) {
      return node;
    }
    int power = 31 - __builtin_clz(k);
    k &= ~(1u << power);
    node = liftings[node][power];
    auto index = matching[node];
    return longestPaths[index.first][index.second + k];
  }

  void macromicro() {
    d.resize(n, 0);
    liftings.resize(n);

    ptr2child.resize(n, 0);
    isLeaf.resize(n, false);
    microDepth.resize(n, -1);
    isPruned.resize(n, false);
    microTreeType.resize(n, 0u);
    microRoot.resize(n, -1);
    vertexEuler.resize(n, -1);
    precalc.resize(1u << (2 * B));

    vector<int> path;
    pruneTrees(0, path);
  }

  int pruneTrees(int vertex, vector<int>& curPath) {
    d[vertex] = curPath.size();
    curPath.push_back(vertex);

    int subtreeSize = 1;
    bool isCurLeaf = true;
    for (int child: children[vertex]) {
      subtreeSize += pruneTrees(child, curPath);
      if (!isPruned[child]) {
        isCurLeaf = false;
        ptr2child[vertex] = ptr2child[child];
      }
    }
    isLeaf[vertex] = isCurLeaf;

    isPruned[vertex] = subtreeSize <= B;
    if (!isPruned[vertex]) {
      // here we create binary lifting array for leaf node
      if (isLeaf[vertex]) {
        ptr2child[vertex] = vertex;

        int offset = 1;
        while (offset < curPath.size()) {
          liftings[vertex].push_back(curPath[curPath.size() - offset - 1]);
          offset *= 2;
        }
      }

      for (int child: children[vertex]) {
        if (!isPruned[child]) continue;

        uint32_t code = 0;  // euler tour in binary digit system
        int pos = 0;
        setPrunedInfo(child, vertex, 0, code, pos, child);
        microTreeType[child] = code;

        if (precalc[code].empty()) {
          precalc[code].resize(B, vector<int> (B, -1));
          stack<int> path;
          int eulerNumber = 0;
          makePrecalc(child, code, eulerNumber, path);
        }
      }
    }

    curPath.pop_back();
    return subtreeSize;
  }

  void setPrunedInfo(int vertex, int ptr, int depth, uint32_t& code, int& pos, int root) {
    ++pos;
    ptr2child[vertex] = ptr;
    microDepth[vertex] = depth;
    microRoot[vertex] = root;
    for (int child: children[vertex]) {
      setPrunedInfo(child, ptr, depth + 1, code, pos, root);
    }
    code |= (1u << pos++);
  }

  void makePrecalc(int vertex, int code, int& eulerNumber, stack<int>& path) {
    path.push(vertex);

    vertexEuler[vertex] = eulerNumber;
    stack<int> poped;
    int going = vertex;
    for (int level = 0; isPruned[going]; ++level) {
      precalc[code][eulerNumber][level] = vertexEuler[path.top()];
      poped.push(path.top());
      path.pop();
      going = parents[going];
    }
    while (!poped.empty()) {
      path.push(poped.top());
      poped.pop();
    }

    for (int child: children[vertex]) {
      makePrecalc(child, code, ++eulerNumber, path);
    }
    path.pop();
  }

  int createPaths(int vertex, int depth) {
    int maxima = depth;
    int continuation;
    for (int child: children[vertex]) {
      if (isPruned[child]) continue;

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
  const int B;
  vector<int> d;  // distnce from root
  vector<vector<int>> liftings;
  vector<int> parents;
  vector<vector<int>> children;
  vector<vector<int>> longestPaths;
  vector<pair<int, int>> matching;  // global and local indexes in paths decomposition
  vector<int> ptr2child;
  vector<char> isLeaf;
  vector<char> isPruned;  // true or false
  vector<int> microDepth;  // depth from root in a microTree
  vector<uint32_t> microTreeType;  // euler tour of vertex's microtree
  vector<int> microRoot;  // root of current vertex in it's microtree
  vector<vector<vector<int>>> precalc;  // code -> vertex -> height
  vector<int> vertexEuler;  // number of vertex in Euler tour
};


// solution for https://leetcode.com/problems/kth-ancestor-of-a-tree-node/
int main() {
  auto x = vector<int>{-1, 0, 1, 2, 3, 3, 3, 2, 7, 8, 9, 9, 9, 8, 1, 14, 15, 15, 17, 0, 19, 20, 20, 0, 0, 24, 25, 26, 27, 27, 29, 27};
  TreeAncestor* treeAncestor = new TreeAncestor(32, x);
  auto y = treeAncestor->precalc[212];
  cout << treeAncestor->getKthAncestor(3, 1) << '\n';
  cout << treeAncestor->getKthAncestor(5, 2) << '\n';
  cout << treeAncestor->getKthAncestor(6, 3) << '\n';
  delete treeAncestor;
}
