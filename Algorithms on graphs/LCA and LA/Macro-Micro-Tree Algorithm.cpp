#include <iostream>
#include <vector>
#include <cmath>
#include <stack>

using namespace std;


// solution for https://leetcode.com/problems/kth-ancestor-of-a-tree-node/
// this problem has weak tests. the solution for the problem with more solid tests provided below
class TreeAncestor {
 public:
  TreeAncestor(int n, vector<int>& p): n(n), B(n == 1? 1: (32 - __builtin_clz(n - 1)) / 4) {
    parents = p;

    children.resize(n, vector<int>());
    for (int i = 0; i < n; ++i) {
      if (p[i] != -1) {
        children[p[i]].push_back(i);
      } else {
        treeRoot = i;
      }
    }

    macromicro();
    matching.resize(n, {0, 0});
    createPaths(treeRoot, 0);
    createLadders();
  }

  int getKthAncestor(int node, int k) {
    if (k > d[node]) {
      return -1;
    }
    if (isPruned[node] == 1) {
      int microDepth = d[node] - d[microRoot[node]];
      if (microDepth >= k) {
        int root = microRoot[node];
        return eulerDecode[root][precalc[microTreeType[root]][eulerTour[node]][k]];
      } else {
        k -= microDepth + 1;
        node = parents[microRoot[node]];
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
    isPruned.resize(n, 2);  // 0 - nonPruned, 1 - pruned, 2 - in process
    microTreeType.resize(n, 0u);
    microRoot.resize(n, -1);
    eulerTour.resize(n, -1);
    precalc.resize(1u << (2 * B));

    vector<int> path;
    pruneTrees(treeRoot, path);

    eulerDecode.resize(n);
    findPrunedRoots(treeRoot);
  }

  int pruneTrees(int vertex, vector<int>& curPath) {
    d[vertex] = curPath.size();
    curPath.push_back(vertex);

    int subtreeSize = 1;
    bool isCurLeaf = true;
    for (int child: children[vertex]) {
      subtreeSize += pruneTrees(child, curPath);
      if (isPruned[child] == 0) {
        isCurLeaf = false;
        ptr2child[vertex] = ptr2child[child];
      }
    }
    isLeaf[vertex] = isCurLeaf;

    isPruned[vertex] = subtreeSize <= B;
    if (isPruned[vertex] == 0) {
      // here we create binary lifting array for the leaf node
      if (isLeaf[vertex]) {
        ptr2child[vertex] = vertex;

        int offset = 1;
        while (offset < curPath.size()) {
          liftings[vertex].push_back(curPath[curPath.size() - offset - 1]);
          offset *= 2;
        }
      }
    }

    curPath.pop_back();
    return subtreeSize;
  }

  void findPrunedRoots(int vertex) {
    if (isPruned[vertex] == 1 && parents[vertex] != -1 && isPruned[parents[vertex]] == 0) {
      uint32_t mask = 0;  // Euler tour in binary digit system
      int pos = 0;
      int euler = 0;
      eulerDecode[vertex].resize(B, -1);
      setInfoInPrunedTree(vertex, 0, mask, pos, euler, vertex);
      microTreeType[vertex] = mask;

      if (precalc[mask].empty()) {
        precalc[mask].resize(B, vector<int>(B, -1));
        stack<int> path;
        makePrecalc(vertex, mask, path);
      }
    } else if (isPruned[vertex] == 0) {
      for (int child: children[vertex]) {
        findPrunedRoots(child);
      }
    }
  }

  void setInfoInPrunedTree(int vertex, int depth, uint32_t& mask, int& pos, int& eulerNumber, int root) {
    ++pos;
    microRoot[vertex] = root;
    eulerTour[vertex] = eulerNumber;
    eulerDecode[root][eulerNumber] = vertex;
    for (int child: children[vertex]) {
      setInfoInPrunedTree(child, depth + 1, mask, pos, ++eulerNumber, root);
    }
    mask |= (1u << pos++);
  }

  void makePrecalc(int vertex, int mask, stack<int>& path) {
    path.push(vertex);

    stack<int> poped;
    int going = vertex;
    for (int level = 0; isPruned[going] == 1; ++level) {
      precalc[mask][eulerTour[vertex]][level] = eulerTour[path.top()];
      poped.push(path.top());
      path.pop();
      going = parents[going];
    }
    while (!poped.empty()) {
      path.push(poped.top());
      poped.pop();
    }

    for (int child: children[vertex]) {
      makePrecalc(child, mask, path);
    }
    path.pop();
  }

  int createPaths(int vertex, int depth) {
    int maxima = depth;
    int continuation;
    for (int child: children[vertex]) {
      if (isPruned[child] == 1) {
        continue;
      }

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
    for (auto& path: longestPaths) {
      int size = longestPaths.size();
      for (int j = 0; j < size && parents[path.back()] != -1; ++j) {
        path.push_back(parents[path.back()]);
      }
    }
  }

  int n;
  const int B;  // microtree trashhold
  vector<int> d;  // distance from root
  vector<vector<int>> liftings;  // binary liftings for the leafs
  vector<int> parents;
  vector<vector<int>> children;
  vector<vector<int>> longestPaths;  // decomposition
  vector<pair<int, int>> matching;  // global and local indexes in paths decomposition
  vector<int> ptr2child;  // pointer to any non-pruned leaf node in the subtree
  vector<char> isLeaf;
  vector<char> isPruned;  // true or false or unprocessed
  vector<uint32_t> microTreeType;  // Euler tour of vertex's microtree (mask)
  vector<int> microRoot;  // root of current vertex in it's microtree
  vector<vector<vector<int>>> precalc;  // mask -> vertex -> height
  vector<int> eulerTour;  // number of the vertex in the Euler tour
  vector<vector<int>> eulerDecode;  // root -> euler number
  int treeRoot;
};


int n;
vector<vector<int>> children;
vector<int> p;
vector<int> d;

// solution for https://codeforces.com/group/QmrArgR1Jp/contest/279285/problem/A
int main() {
  std::cin.tie(nullptr);
  std::ios_base::sync_with_stdio(false);

  std::cin >> n;
  p.resize(n, 0);
  d.resize(n, 0);

  children.resize(n, std::vector<int>());

  d[0] = 0;
  // read parents
  int index = -1;
  for (int i = 0; i < n; ++i) {
    std::cin >> p[i];
    --p[i];
  }
  TreeAncestor t(n, p);
  for (int j = 0; j < n; ++j) {
    int i = j;
    if (j < index) {
      ++i;
    } else if (j == index) {
      i = 0;
    }
    int k = 1;
    int res = t.getKthAncestor(i, k);
    cout << j + 1 << ": ";
    while (res != -1) {
      cout << res + 1 << ' ';
      k *= 2;
      res = t.getKthAncestor(i, k);
    }
    cout << '\n';
  }
}
