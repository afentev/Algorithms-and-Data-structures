#include <iostream>
#include <vector>
#include <chrono>
#include <map>


template<typename K, typename V>
class AVL {
 private:
  struct Node {
    Node(const K& k, const V& v) : left(nullptr), right(nullptr), key(k), val(v) {}

    Node* left;
    Node* right;
    size_t height;
    K key;
    V val;
  };

  void fixHeight(Node* vertex) {
    size_t hLeft = 0;
    size_t hRight;
    if (vertex->left != nullptr) {
      hLeft = vertex->left->height;
    }
    if (vertex->right != nullptr) {
      hRight = vertex->right->height;
    }
    vertex->height = std::max(hLeft, hRight) + 1;
  }

  Node* rightRotation(Node* vertex) {
    Node* child = vertex->left;
    if (child == nullptr) {
      return vertex;
    }
    vertex->left = child->right;
    child->right = vertex;
    fixHeight(vertex);
    fixHeight(child);
    return child;
  }

  Node* leftRotation(Node* vertex) {
    Node* child = vertex->right;
    if (child == nullptr) {
      return vertex;
    }
    vertex->right = child->left;
    child->left = vertex;
    fixHeight(vertex);
    fixHeight(child);
    return child;
  }

  int disbalance(Node* vertex) {
    int hLeft = 0;
    int hRight = 0;
    if (vertex->left != nullptr) {
      hLeft = vertex->left->height;
    }
    if (vertex->right != nullptr) {
      hRight = vertex->right->height;
    }
    return hRight - hLeft;
  }

  Node* rebalance(Node* vertex) {
    fixHeight(vertex);
    int disb = disbalance(vertex);
    if (disb == 2) {
      if (disbalance(vertex->right) < 0) {
        vertex->right = rightRotation(vertex->right);
      }
      return leftRotation(vertex);
    } else if (disb == -2) {
      if (disbalance(vertex->left) > 0) {
        vertex->left = leftRotation(vertex->left);
      }
      return rightRotation(vertex);
    }
    return vertex;
  }

  void remover(Node* vertex) {
    if (vertex == nullptr) {
      return;
    }
    remover(vertex->left);
    remover(vertex->right);
    delete vertex;
  }

  Node* eraseMin(Node* p) {
    if (p->left == 0) {
      return p->right;
    }
    p->left = eraseMin(p->left);
    return rebalance(p);
  }

  Node* erase(const K& key, Node* vertex) {
    if (vertex == nullptr) {
      return vertex;
    }
    if (vertex->key < key) {
      vertex->right = erase(key, vertex->right);
    } else if (vertex->key > key) {
      vertex->left = erase(key, vertex->left);
    } else {
      Node* left = vertex->left;
      Node* right = vertex->right;
      delete vertex;

      if (right == nullptr) {
        return left;
      }
      Node* dig = right;
      while (dig->left != nullptr) {
        dig = dig->left;
      }
      dig->right = eraseMin(right);
      dig->left = left;
      return rebalance(dig);
    }
    return rebalance(vertex);
  }

  // 1st - techincal Node* for rebalance
  // 2st - actual lookup return value: found or inserted vertex
  // 3st - is rebalance needed
  std::tuple<Node*, Node*, bool> lookup(const K& key, Node* vertex) {
    if (vertex == nullptr) {
      Node* created = new Node(key, V());
      return {created, created, true};
    }
    std::tuple<Node*, Node*, bool> result;
    if (vertex->key < key) {
      result = lookup(key, vertex->right);
      vertex->right = std::get<0>(result);
    } else if (vertex->key > key) {
      result = lookup(key, vertex->left);
      vertex->left = std::get<0>(result);
    } else {
      return {vertex, vertex, false};
    }
    if (std::get<2>(result)) {
      size_t old = vertex->height;
      Node* ret = rebalance(vertex);
      size_t current = ret->height;
      return {ret, std::get<1>(result), old != current};
    }
    return {vertex, std::get<1>(result), false};
  }

 private:
  Node* root;

 public:
  AVL() {
    root = nullptr;
  }

  ~AVL() {
    remover(root);
  }

  void erase(const K& key) {
    root = erase(key, root);
  }

  V& operator[](const K& key) {
    auto result = lookup(key, root);
    root = std::get<0>(result);
    return std::get<1>(result)->val;
  }

  V& lookup(const K& key) {
    Node* vertex = root;
    while (true) {
      if (vertex->key < key) {
        vertex = vertex->right;
      } else if (vertex->key > key) {
        vertex = vertex->left;
      } else {
        return vertex->val;
      }
    }
  }

  const V& operator[](const K& key) const {
    return lookup(key);
  }
};


void runTests() {
  AVL<int, int> t;
  std::map<int, int> m;

  int N = 1e6;
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
  AVL<int, int> testAVL;
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
    testAVL[keys[i]] = values[i];
  }
  auto end = std::chrono::high_resolution_clock::now();
  std::cout << "AVL tree insertion: " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count()
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
    volatile int val = testAVL.lookup(keys[i]);
  }
  end = std::chrono::high_resolution_clock::now();
  std::cout << "AVL tree lookup: " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count()
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
    testAVL.erase(keys[i]);
  }
  end = std::chrono::high_resolution_clock::now();
  std::cout << "AVL tree erase: " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count()
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
