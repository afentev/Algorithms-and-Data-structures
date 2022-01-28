#include <iostream>
#include <vector>
#include <chrono>
#include <cassert>
#include <algorithm>


template <typename K, typename V>
class BTree {
 private:
  struct Node {
    struct Element {
      Element(const K& key, const V& val): key(key), val(val), child(nullptr) {}
      Element(): child(nullptr) {}

      bool operator<(int k) const {
        return key < k;
      }

      K key;
      V val;
      Node* child;
    };

    Node() {}
    Node(const K& key, const V& val, bool isLeaf = false): data({Element(key, val)}), isLeaf(isLeaf) {
      data.reserve(2 * t - 1);
      data.emplace_back();
    };

    size_t size() {
      if (data.empty()) {
        return 0;
      }
      return data.size() - 1;
    }

    std::vector<Element> data;
    bool isLeaf = false;
  };

  void remover(Node* ptr) {
    if (ptr == nullptr) {
      return;
    }
    for (auto x: ptr->data) {
      remover(x.child);
    }
    delete ptr;
  }

 public:
  BTree() {
    root = nullptr;
  }

  ~BTree() {
    remover(root);
  }

  void insert(const K& key, const V& val) {
    if (root == nullptr) {
      root = new Node(key, val);
      root->isLeaf = true;
      return;
    }
    if (root->size() == 2 * t - 1) {
//      Node* newRoot = new Node(root->data[t - 1].key, root->data[t - 1].val);
      Node* rightSibling = new Node();
      for (size_t index = root->data.size() - 1; index > t - 1; --index) {
        rightSibling->data.push_back(root->data.back());
        root->data.pop_back();
      }
      std::reverse(rightSibling->data.begin(), rightSibling->data.end());
      auto k = std::prev(root->data.end());
      Node* newRoot = new Node(k->key, k->val);

      newRoot->data[0].child = root;
      newRoot->data[1].child = rightSibling;
      rightSibling->isLeaf = root->isLeaf;
      root = newRoot;
      root->isLeaf = false;
    }
    Node* parent = nullptr;
    Node* ptr = root;
    while (true) {
      if (ptr->size() == 2 * t - 1) {
        auto p = std::lower_bound(parent->data.begin(), parent->data.end() - 1, ptr->data[t - 1].key);
        size_t ind = std::distance(parent->data.begin(), p);
        parent->data.insert(p, ptr->data[t - 1]);
        parent->data[ind].child = ptr;

        Node* rightSibling = new Node();
        for (size_t index = ptr->data.size() - 1; index > t - 1; --index) {
          rightSibling->data.push_back(ptr->data.back());
          ptr->data.pop_back();
        }
        std::reverse(rightSibling->data.begin(), rightSibling->data.end());
        parent->data[ind + 1].child = rightSibling;
        rightSibling->isLeaf = ptr->isLeaf;
        parent->isLeaf = false;

        if (key > parent->data[ind].key) {
          ptr = rightSibling;
        }
      }
      if (ptr->isLeaf) {
        break;
      }
      parent = ptr;
      ptr = std::lower_bound(ptr->data.begin(), ptr->data.end() - 1, key)->child;
    }

    ptr->data.insert(std::lower_bound(ptr->data.begin(), ptr->data.end() - 1, key), typename Node::Element(key, val));
  }

  const V& get(const K& key) const {
    Node* ptr = root;
    while (ptr != nullptr) {
      auto p = std::lower_bound(ptr->data.begin(), ptr->data.end() - 1, key);
      if (p->key == key) {
        return p->val;
      }
      ptr = p->child;
    }
  }

  void erase(const K& key) const {
    if (root->isLeaf) {
      if (root->size() == 1) {
        delete root;
        root = nullptr;
      } else {
        root->data.erase(std::lower_bound(root->data.begin(), root->data.end(), key));
      }
      return;
    }
    if (root->size() == 1) {
      Node* rightSibling = new Node();
      for (size_t index = root->data.size() - 1; index > t - 1; --index) {
        rightSibling->data.push_back(root->data.back());
        root->data.pop_back();
      }
      std::reverse(rightSibling->data.begin(), rightSibling->data.end());
      auto k = std::prev(root->data.end());
      Node* newRoot = new Node(k->key, k->val);

      newRoot->data[0].child = root;
      newRoot->data[1].child = rightSibling;
      rightSibling->isLeaf = root->isLeaf;
      root = newRoot;
      root->isLeaf = false;
    }
    Node* parent = nullptr;
    Node* ptr = root;
    while (true) {

    }
  }

  void traverse(Node* p) {
    if (p == nullptr) return;
    for (auto x: p->data) {
      traverse(x.child);
    }
    std::cout << p->data.size() - 1 << ' ' << p->isLeaf << '\n';
  }

 private:
  Node* root;
  static const int t = 30;
};

#include <map>
void runTests() {
  BTree<int, int> t;
  std::map<int, int> m;

  int N = 1e3;
  std::vector<int> d, r;
  d.reserve(N);

  for (int i = 0; i < N; ++i) {
    int k = rand();
    int v = rand();
    t.insert(k, v);
    m[k] = v;
  }
  int ind = 0;
  for (auto x: m) {
    assert(x.second == t.get(x.first));
  }
  std::cout << "All tests passed!\n" << std::endl;

//  t.traverse(t.root);
}

void runPerfTests() {
  const int N = 1e6;
  BTree<int, int> testAVL;
  std::map<int, int> testM;

  std::vector<int> keys, values;
  keys.reserve(N);
  values.reserve(N);
  for (int i = 0; i < N; ++i) {
    keys.push_back(rand());
    values.push_back(rand());
  }

  // -----------------------------------------------------------------------------------------------------------------
  auto begin = std::chrono::high_resolution_clock::now();
  for (int i = 0; i < N; ++i) {
    testAVL.insert(keys[i], values[i]);
  }
  auto end = std::chrono::high_resolution_clock::now();
  std::cout << "B-tree insertion: " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count()
            << "ms" << std::endl;

  begin = std::chrono::high_resolution_clock::now();
  for (int i = 0; i < N; ++i) {
    testM[keys[i]] = values[i];
  }
  end = std::chrono::high_resolution_clock::now();
  std::cout << "Map insertion: " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() << "ms\n"
            << std::endl;

  // -----------------------------------------------------------------------------------------------------------------

  begin = std::chrono::high_resolution_clock::now();
  for (int i = 0; i < N; ++i) {
    volatile int val = testAVL.get(keys[i]);
  }
  end = std::chrono::high_resolution_clock::now();
  std::cout << "B-tree lookup: " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count()
            << "ms" << std::endl;

  begin = std::chrono::high_resolution_clock::now();
  for (int i = 0; i < N; ++i) {
    volatile int val = testM[keys[i]];
  }
  end = std::chrono::high_resolution_clock::now();
  std::cout << "Map lookup: " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() << "ms\n"
            << std::endl;

}

int main() {
  runTests();
  runPerfTests();
  return 0;
}
