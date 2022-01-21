#include <iostream>
#include <vector>
#include <chrono>
#include <map>


template<typename K, typename V>
class AVL {
 private:
  struct Node {
    Node(const K& k, const V& v) : left(nullptr), right(nullptr), parent(nullptr), key(k), val(v) {}

    Node* left;
    Node* right;
    Node* parent;
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
  // 2st - actual get's return value: found or inserted vertex
  std::pair<Node*, Node*> get(const K& key, Node* vertex) {
    if (vertex == nullptr) {
      Node* created = new Node(key, V());
      return {created, created};
    }
    std::pair<Node*, Node*> result;
    if (vertex->key < key) {
      result = get(key, vertex->right);
      vertex->right = result.first;
    } else if (vertex->key > key) {
      result = get(key, vertex->left);
      vertex->left = result.first;
    } else {
      return {vertex, vertex};
    }
    return {rebalance(vertex), result.second};
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
    auto result = get(key, root);
    root = result.first;
    return result.second->val;
  }
};


int main() {
  AVL<int, int> t;
  std::map<int, int> m;

  int N = 1e6;
  std::vector<int> d;
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

  std::cout << "All tests passed!" << std::endl;

  AVL<int, int> testST;
  std::map<int, int> testM;
  auto begin = std::chrono::high_resolution_clock::now();
  for (int i = 0; i < N; ++i) {
    int k = rand();
    int v = rand();
    testST[k] = v;
  }
  auto end = std::chrono::high_resolution_clock::now();
  std::cout << "Splay tree insertion: " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count()
            << "ms" << std::endl;

  begin = std::chrono::high_resolution_clock::now();
  for (int i = 0; i < N; ++i) {
    int k = rand();
    int v = rand();
    testM[k] = v;
  }
  end = std::chrono::high_resolution_clock::now();
  std::cout << "Map insertion: " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() << "ms"
            << std::endl;
  return 0;
}
