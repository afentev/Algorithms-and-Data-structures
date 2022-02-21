#include <iostream>
#include <vector>
#include <cassert>
#include <map>
#include <chrono>


template<typename K, typename V>
class SplayTree {
  struct Node {
    Node(const K& key, const V& val) : left(nullptr), right(nullptr), parent(nullptr),
                                                   key(key), val(val) {}

    Node* left, *right, *parent;
    K key;
    V val;
  };

  void leftRotation(Node* x) {
    Node* y = x->right;
    if (y != nullptr) {
      x->right = y->left;
      if (y->left != nullptr) {
        y->left->parent = x;
      }
      y->parent = x->parent;
    }

    replace(x, y);
    if (y != nullptr) {
      y->left = x;
    }
    x->parent = y;
  }

  void rightRotation(Node* x) {
    Node* y = x->left;
    if (y != nullptr) {
      x->left = y->right;
      if (y->right != nullptr) {
        y->right->parent = x;
      }
      y->parent = x->parent;
    }
    replace(x, y);
    if (y != nullptr) {
      y->right = x;
    }
    x->parent = y;
  }

  void splay(Node* vertex) {
    if (vertex->parent == nullptr) {
      return;
    }
    Node* parent = vertex->parent;
    if (parent->parent == nullptr) {  // "zig"
      if (parent->left == vertex) {
        rightRotation(vertex->parent);
      } else {
        leftRotation(vertex->parent);
      }
      return;
    }
    Node* dad = parent->parent;

    if (dad->left == parent) {
      if (parent->left == vertex) {  // "zig-zig"
        rightRotation(vertex->parent->parent);
      } else {  // "zig-zag"
        leftRotation(vertex->parent);
      }
      rightRotation(vertex->parent);
    } else {
      if (parent->left == vertex) {  // "zig-zag"
        rightRotation(vertex->parent);
      } else {  // "zig-zig"
        leftRotation(vertex->parent->parent);
      }
      leftRotation(vertex->parent);
    }
    splay(vertex);
  }

  void remover(Node* vertex) {
    if (vertex != nullptr) {
      remover(vertex->left);
      remover(vertex->right);
      delete vertex;
    }
  }

 private:
  Node* root;

 public:
  SplayTree(): root(nullptr) {}

  ~SplayTree() {
    remover(root);
  }

  void replace(Node* u, Node* v) {
    if (u->parent == nullptr) {
      root = v;
    } else if (u == u->parent->left) {
      u->parent->left = v;
    } else {
      u->parent->right = v;
    }
    if (v != nullptr) {
      v->parent = u->parent;
    }
  }

  void erase(const K& key) {
    Node* target = find(key);
    if (target == nullptr) {
      return;
    }

    splay(target);

    if (!target->left) {
      root = target->right;
      if (target->right != nullptr) {
        target->right->parent = target->parent;
      }
    } else if (!target->right) {
      root = target->left;
      if (target->left != nullptr) {
        target->left->parent = target->parent;
      }
    } else {
      Node* minima = target->right;
      while (minima->left) {
        minima = minima->left;
      }
      if (minima->parent != target) {
        replace(minima, minima->right);
        minima->right = target->right;
        minima->right->parent = minima;
      }
      replace(target, minima);
      minima->left = target->left;
      minima->left->parent = minima;
    }

    if (target == root) {
      delete target;
      root = nullptr;
    } else {
      delete target;
    }
  }

  Node* find(const K& key) {
    Node* dig = root;
    while (dig != nullptr) {
      if (dig->key < key) {
        dig = dig->right;
      } else if (key < dig->key) {
        dig = dig->left;
      } else {
        return dig;
      }
    }
    return nullptr;
  }

  V& operator[](const K& key) {
    Node* dig = root;
    Node* parent = nullptr;
    bool direction = true;
    while (dig) {
      parent = dig;
      if (dig->key > key) {
        dig = dig->left;
        direction = true;
      } else if (dig->key < key) {
        dig = dig->right;
        direction = false;
      } else {
        splay(dig);
        return dig->val;
      }
    }

    Node* vertex = new Node(key, V());
    vertex->parent = parent;
    if (parent == nullptr) {
      root = vertex;
    } else if (direction) {
      parent->left = vertex;
    } else {
      parent->right = vertex;
    }

    splay(vertex);
    return vertex->val;
  }

  V& operator[](const K& key) const {
    return lookup(key);
  }
};


void runTests() {
  SplayTree<int, int> t;
  std::map<int, int> m;

  int N = 1e5;
  std::vector<int> d, r;
  d.reserve(N);

  for (int i = 0; i < N; ++i) {
    int k = rand();
    int v = rand();
    t[k] = v;
    m[k] = v;
  }
  for (auto x: m) {
    assert(x.second == t[x.first]);
    if (rand() % 2 == 1) {
      d.push_back(x.first);
    }
  }
  for (int x: d) {
    m.erase(x);
    t.erase(x);
  }
  for (auto x: m) {
    assert(x.second == t[x.first]);
  }

  std::cout << "All tests passed!\n" << std::endl;
}

void runPerfTests() {
  const int N = 1e6;
  SplayTree<int, int> testST;
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
    testST[keys[i]] = values[i];
  }
  auto end = std::chrono::high_resolution_clock::now();
  std::cout << "Splay tree insertion: " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count()
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
    volatile int val = testST[keys[i]];
  }
  end = std::chrono::high_resolution_clock::now();
  std::cout << "Splay tree lookup: " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count()
            << "ms" << std::endl;

  begin = std::chrono::high_resolution_clock::now();
  for (int i = 0; i < N; ++i) {
    volatile int val = testM[keys[i]];
  }
  end = std::chrono::high_resolution_clock::now();
  std::cout << "Map lookup: " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() << "ms\n"
            << std::endl;

  // -----------------------------------------------------------------------------------------------------------------

  begin = std::chrono::high_resolution_clock::now();
  for (int i = 0; i < N; ++i) {
    testST.erase(keys[i]);
  }
  end = std::chrono::high_resolution_clock::now();
  std::cout << "Splay tree erase: " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count()
            << "ms" << std::endl;

  begin = std::chrono::high_resolution_clock::now();
  for (int i = 0; i < N; ++i) {
    testM.erase(keys[i]);
  }
  end = std::chrono::high_resolution_clock::now();
  std::cout << "Map erase: " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() << "ms\n"
            << std::endl;

}

int main() {
  runTests();
  runPerfTests();

  return 0;
}