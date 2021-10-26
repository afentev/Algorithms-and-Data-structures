//
// Created by afentev on 9/28/21.
//

#include <iostream>
#include <queue>
#include <tuple>
#include <chrono>
#include <random>

using std::cout, std::cin, std::endl;

template<class TKey, class Tval>
class PairingHeapImpl_ {
 public:
  PairingHeapImpl_() {
    initialized = false;
  }

  PairingHeapImpl_* link(PairingHeapImpl_* h1, PairingHeapImpl_* h2) {
    if (h1 == nullptr || !h1->initialized) {
      return h2;
    } else if (h2 == nullptr || !h2->initialized) {
      return h1;
    }
    if (h1->key < h2->val) {
      h2->next = h1->child;
      h1->child = h2;
      return h1;
    } else {
      h1->next = h2->child;
      h2->child = h1;
      return h2;
    }
  }

  PairingHeapImpl_* merge(PairingHeapImpl_* h1, PairingHeapImpl_* h2 = nullptr) {
    // assert(h1 == nullptr || !h1->initialized || h2 == nullptr || !h2->initialized);
    if (h1 == nullptr || !h1->initialized) {
      std::swap(h1, h2);
    }
    std::queue<PairingHeapImpl_*> q;
    while (h1 != nullptr) {
      if (h1->next == nullptr) {
        q.push(h1);
        h1 = h1->next;  // break
      } else {
        next = h1->next->next;
        q.push(h1->link(h1, h1->next));
        h1 = next;
      }
    }
    h1 = q.front();
    q.pop();
    while (!q.empty()) {
      h1 = h1->link(h1, q.front());
      q.pop();
    }
    return h1;
  }

  Tval getMinimum() {
    return val;
  }

  PairingHeapImpl_* insert(TKey k, Tval v) {
    auto* appendix = new PairingHeapImpl_;
    appendix->initialized = true;
    appendix->key = k;
    appendix->val = v;
    return link(this, appendix);
  }

  std::tuple<PairingHeapImpl_*, PairingHeapImpl_*, Tval> extractMin() {
    if (child == nullptr) {
      auto* newHeap = new PairingHeapImpl_;
      return {this, newHeap, val};
    }
    return {this, merge(child), val};
  }

  PairingHeapImpl_* child = nullptr;
  PairingHeapImpl_* next = nullptr;
  TKey key;
  Tval val;
  bool initialized = false;
};

template<class TKey, class Tval>
class PairingHeap {
 public:
  PairingHeap() {
    h = new PairingHeapImpl_<TKey, Tval>;
  }

  void insert(TKey k, Tval v) {
    h = h->insert(k, v);
  }

  Tval extractMin() {
    auto x = h->extractMin();
    delete std::get<0>(x);  // <- very slow
    h = std::get<1>(x);
    return std::get<2>(x);
  }
  PairingHeapImpl_<TKey, Tval>* h;
};

int main() {
  int n = 1e6;

  std::random_device rd;
  std::mt19937 g(rd());
  std::vector<int> x(n);

  for (int i = 0; i < n; ++i) {
    x[i] = i;
  }

  PairingHeap<int, int> t;

  std::shuffle(x.begin(), x.end(), g);
  std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
  for (int i = 0; i < n; ++i) {
    t.insert(x[i], x[i]);
  }

  for (int i = 0; i < n; ++i) {
    t.extractMin();
  }
  std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
  std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count()
            << "[ms]" << std::endl;

  return 0;
}
