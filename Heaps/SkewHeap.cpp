#include <iostream>
#include <random>
#include <chrono>


template <typename T>
class SkewHeap {
 private:
  struct Node {
    Node(const T& k): left(nullptr), right(nullptr), key(k) {}

    Node* left;
    Node* right;
    T key;
  };

  void remover(Node* ptr) {
    if (ptr == nullptr) {
      return;
    }
    remover(ptr->left);
    remover(ptr->right);
    delete ptr;
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
    Node* temp = left->right;
    left->right = left->left;
    left->left = merge(temp, right);
    return left;
  }

 private:
  Node* root;

 public:
  SkewHeap() {
    root = nullptr;
  }

  ~SkewHeap() {
    remover(root);
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

  SkewHeap<int> t;

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
