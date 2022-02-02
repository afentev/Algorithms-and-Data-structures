#include <iostream>
#include <vector>
#include <map>

const int u = INT32_MAX / 2.718281828459045;

bool coinFlip() {
  return rand() < u;
//  return rand() % 2;
}


template <typename K, typename V, K minima, K maxima>
class SkipList {
  struct Node {
    Node(const K& k, const V& v = V(), Node* nxt = nullptr, Node* under = nullptr, size_t w = 1):
        key(k),
        val(v),
        next(nxt),
        bot(under),
        width(w)
    {}

    K key;
    V val;
    Node* next = nullptr;
    Node* bot = nullptr;
    size_t width = 1;
  };

  Node* end = new Node(maxima);
  Node* begin = new Node(minima, V(), end, nullptr, 0);
  size_t size = 0;

 private:
  std::pair<bool, Node*> insert(const K& key, const V& val, Node* cur) {
    if (cur != nullptr) {
      while (cur->next->key != maxima && key >= cur->next->key) {
        cur = cur->next;
      }
      auto res = insert(key, val, cur->bot);

      if (res.first) {
        Node* node = new Node{key, val, cur->next, res.second, 1};
        if (node->bot != nullptr) {
          node->width--;
          Node* ptr = cur->bot;
          while (ptr != res.second) {
            ptr = ptr->next;
            node->width += ptr->width;
          }
        }
        cur->next = node;
        node->next->width -= node->width - 1;

        return {coinFlip(), node};
      }
      ++cur->next->width;
      return {false, res.second};
    } else {
      return {true, nullptr};
    }
  }

 public:
  void insert(const K& key, const V& val) {
    size++;
    auto newLevel = insert(key, val, begin);
    if (newLevel.first) {
      Node* newEnd = new Node(maxima, V(), nullptr, end, end->width);
      Node* node = new Node(key, val, newEnd, newLevel.second, 0);
      Node* newBegin = new Node(minima, V(), node, begin, 0);

      Node* ptr = begin;
      while (ptr != newLevel.second) {
        ptr = ptr->next;
        node->width += ptr->width;
      }

      newEnd->width = size - node->width + 1;

      begin = newBegin;
      end = newEnd;
    }
  }

  void erase(const K& key) {
    size--;
    Node* cur = begin;
    while (cur != nullptr) {
      while (cur->next->key != maxima && key > cur->next->key) {
        cur = cur->next;
      }
      if (key == cur->next->key) {
        cur->next->next->width += cur->next->width - 1;
        Node* toDelete = cur->next;
        cur->next = cur->next->next;
        delete toDelete;
      } else {
        --cur->next->width;
      }
      if (cur->key == minima && cur->next->key == maxima) {  // erase empty level
        begin = cur->bot;
        end = cur->next->bot;
        Node* toDelete = cur;
        cur = cur->bot;
        delete toDelete->next;
        delete toDelete;
      } else {
        cur = cur->bot;
      }
    }
  }

  V get(const K& key) const {
    Node* cur = begin;
    while (cur->bot != nullptr) {
      while (cur->next->key != maxima && key >= cur->next->key) {
        cur = cur->next;
      }
      cur = cur->bot;
    }
    while (key >= cur->next->key) {
      cur = cur->next;
    }
    return cur->val;
  }

  const V& operator[] (size_t index) const {  // k-th smallest
    ++index;
    size_t counter = 0;
    Node* cur = begin;
    while (cur->bot != nullptr) {
      while (counter + cur->next->width <= index) {
        cur = cur->next;
        counter += cur->width;
      }
      cur = cur->bot;
    }
    while (counter + cur->next->width <= index) {
      cur = cur->next;
      counter += cur->next->width;
    }
    return cur->val;
  }
};

void runTests() {
  SkipList<int, int, INT32_MIN, INT32_MAX> t;
  std::map<int, int> m;

  int N = 1e6;
  std::vector<int> d, r;
  d.reserve(N);

  for (int i = 0; i < N; ++i) {
    int k = rand();
    int v = rand();
    t.insert(k, v);
    m[k] = v;
  }
  for (auto x: m) {
    assert(x.second == t.get(x.first));
    if (rand() % 2 == 1) {
      d.push_back(x.first);
    }
  }
  for (int x: d) {
    m.erase(x);
    t.erase(x);
  }
  for (auto x: m) {
    assert(x.second == t.get(x.first));
  }

  size_t ind = 0;
  for (auto x = m.begin(); x != m.end(); ++x) {
    assert(x->second == t[ind]);
    ind++;
  }

  std::cout << "All tests passed!\n" << std::endl;
}

void runPerfTests() {
  const int N = 1e6;
  SkipList<int, int, INT32_MIN, INT32_MAX> testST;
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
    testST.insert(keys[i], values[i]);
  }
  auto end = std::chrono::high_resolution_clock::now();
  std::cout << "Skip list insertion: " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count()
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
    volatile int val = testST.get(keys[i]);
  }
  end = std::chrono::high_resolution_clock::now();
  std::cout << "Skip list lookup: " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count()
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
  std::cout << "Skip list erase: " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count()
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
