#include <vector>
#include <numeric>
#include <iostream>

using namespace std;

struct DSU {
  DSU(size_t size_ = 0) {
    init(size_);
  }

  void init(size_t size_) {
    parent.resize(size_, 0);
    iota(parent.begin(), parent.end(), 0);
    size.resize(size_, 1);
  }

  void make_set(int v) {
    parent[v] = v;
    size[v] = 1;
  }

  void union_sets(int v, int u) {
    union_roots(find_set(v), find_set(u));
  }

  void union_roots(int v, int u) {
    if (v != u) {
      if (size[v] > size[u]) {
        swap(v, u);
      }
      parent[v] = u;
      size[u] += size[v];
    }
  }

  int find_set(int v) {
    if (v == parent[v]) {
      return v;
    }
    return parent[v] = find_set(parent[v]);
  }

  vector<int> parent;
  vector<int> size;
};

// solution for https://informatics.msk.ru/mod/statements/view.php?id=33303&chapterid=3558#1
int main() {
  cin.tie(nullptr);
  ios_base::sync_with_stdio(false);

  DSU dsu;
  string command;
  while (cin >> command) {
    if (command == "RESET") {
      int size;
      cin >> size;
      dsu.init(size);
      cout << "RESET DONE\n";
    } else if (command == "JOIN") {
      int v, u, vRoot, uRoot;
      cin >> v >> u;
      vRoot = dsu.find_set(v);
      uRoot = dsu.find_set(u);
      if (vRoot == uRoot) {
        cout << "ALREADY " << v << ' ' << u << '\n';
      } else {
        dsu.union_roots(vRoot, uRoot);
      }
    } else if (command == "CHECK") {
      int v, u, vRoot, uRoot;
      cin >> v >> u;
      vRoot = dsu.find_set(v);
      uRoot = dsu.find_set(u);
      if (vRoot == uRoot) {
        cout << "YES\n";
      } else {
        cout << "NO\n";
      }
    }
  }
}