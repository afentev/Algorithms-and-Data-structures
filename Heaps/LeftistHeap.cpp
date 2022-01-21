#include <iostream>
#include <random>
#include <chrono>


template <typename T>
class LeftistHeap {
  struct Node {
    Node(const T& k): left(nullptr), right(nullptr), key(k), dist(0) {}

    Node* left;
    Node* right;
    T key;
    size_t dist;
  };

  void remover(Node* ptr) {
    if (ptr == nullptr) {
      return;
    }
    remover(ptr->left);
    remover(ptr->right);
    delete ptr;
  }

 private:
  Node* root;

 public:
  LeftistHeap() {
    root = nullptr;
  }

  ~LeftistHeap() {
    remover(root);
  }

  Node* merge(Node* left, Node* right) {
    if (left == nullptr) {
      return right;
    } else if (right == nullptr) {
      return left;
    }
    if (left->key > right->key) {
      std::swap(left, right);
    }
    left->right = merge(left->right, right);
    if (left->left == nullptr || left->right->dist > left->left->dist) {
      std::swap(left->right, left->left);
    }

    if (left->right == nullptr) {
      left->dist = 1;
    } else {
      left->dist = left->right->dist + 1;
    }
    return left;
  }

  void insert(const T& k) {
    Node* vertex = new Node(k);
    root = merge(root, vertex);
  }

  T getMin() {
    return root->key;
  }

  T extractMin() {
    T val = root->key;
    Node* left = root->left;
    Node* right = root->right;
    delete root;
    root = merge(left, right);
    return val;
  }
};


int main() {
  int n = 1e6;

  std::random_device rd;
  std::mt19937 g(rd());
  std::vector<int> x(n);

  for (int i = 0; i < n; ++i) {
    x[i] = i;
  }

  LeftistHeap<int> t;

  std::shuffle(x.begin(), x.end(), g);
  std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
  for (int i = 0; i < n; ++i) {
    t.insert(x[i]);
  }

  for (int i = 0; i < n; ++i) {
    t.extractMin();
  }
  std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
  std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count()
            << "[ms]" << std::endl;

  return 0;
}
