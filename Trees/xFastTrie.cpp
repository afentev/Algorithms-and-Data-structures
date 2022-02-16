#include <iostream>
#include <unordered_set>
#include <vector>
#include <chrono>
#include <map>
#include <unordered_map>
#include <set>
#include <random>

#include <ext/pb_ds/assoc_container.hpp>

template <typename Val>
struct Node {
  Node() = default;
  Node(const Val& v): max(v), min(v) {}

  Val min;
  Val max;
};

template <size_t w, typename Val>
class xFastTrie {
  using Container = __gnu_pbds::gp_hash_table<Val, Node<Val>>;
//  using Container = std::unordered_map<Val, Node<Val>>;  // also possible but slower

 private:
  size_t getNodeLevel(const Val& v, int lhs = -1, int rhs = w) {
    while (rhs - lhs > 1) {
      int mid = (rhs + lhs) / 2;
      if (mapper[mid].find(v >> (w - mid - 1)) != mapper[mid].end()) {
        lhs = mid;
      } else {
        rhs = mid;
      }
    }
    return lhs;
  }

 public:
  xFastTrie() {
    mapper.resize(w, Container());
  }

  void insert(const Val& v) {
    Val cur = v;
    for (int i = w - 1; i >= 0; --i) {
      auto it = mapper[i].find(cur);
      if (it == mapper[i].end()) {
        mapper[i][cur] = Node(v);
      } else {
        Val& maxima = it->second.max;
        Val& minima = it->second.min;
        if (v < minima) {
          minima = v;
        } else if (v > maxima) {
          maxima = v;
        }
      }
      cur >>= 1;
    }
  }

  void erase(const Val& v) {
    Val cur = v;
    for (int i = w - 1; i >= 0; --i) {
      auto it = mapper[i].find(cur);
      if (it->second.min == it->second.max) {
        mapper[i].erase(cur);
      } else if (v == it->second.min) {
        it->second.min = predecessor(v - 1);
      } else if (v == it->second.max) {
        it->second.max = successor(v + 1);
      }
      it->second;
      cur >>= 1;
    }
  }

  bool contains(const Val& v) {
    return getNodeLevel(v) == w - 1;
  }

  Val successor(const Val& v) {
    size_t lhs = getNodeLevel(v);
    Val minima = mapper[lhs][v >> (w - lhs - 1)].min;
    if (minima >= v) {
      return minima;
    }

    Val maxima = mapper[lhs][v >> (w - lhs - 1)].max;
    return maxima;
  }

  Val predecessor(const Val& v) {
    size_t lhs = getNodeLevel(v);
    Val maxima = mapper[lhs][v >> (w - lhs - 1)].max;
    if (maxima <= v) {
      return maxima;
    }

    Val minima = mapper[lhs][v >> (w - lhs - 1)].min;
    return minima;
  }

  std::vector<Container> mapper;
};

void runTests() {
  xFastTrie<32, uint32_t> t;
  std::set<int> m;

  int N = 1e6;
  std::vector<int> d, r;
  d.reserve(N);

  for (int i = 0; i < N; ++i) {
    int k = rand();
    int v = rand();
    t.insert(k);
    m.insert(k);
    r.push_back(k);
  }
  for (auto x: r) {
    assert(t.contains(x) == m.contains(x));
    if (rand() % 2 == 1) {
      d.push_back(x);
    }
  }
  for (int x: d) {
    m.erase(x);
    t.erase(x);
  }
  for (auto x: r) {
    assert(m.contains(x) == t.contains(x));
  }

  std::cout << "All tests passed!\n" << std::endl;
}


void runPerfTests() {
  const int N = 1e6;
  xFastTrie<32, uint32_t> testXFastTrie;
  std::set<int> testM;

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
    testXFastTrie.insert(keys[i]);
  }
  auto end = std::chrono::high_resolution_clock::now();
  std::cout << "X-fast trie insertion: " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count()
            << "ms" << std::endl;

  begin = std::chrono::high_resolution_clock::now();
  for (int i = 0; i < N; ++i) {
    testM.insert(keys[i]);
  }
  end = std::chrono::high_resolution_clock::now();
  std::cout << "Set insertion: " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() << "ms\n"
            << std::endl;

  // -----------------------------------------------------------------------------------------------------------------

  begin = std::chrono::high_resolution_clock::now();
  for (int i = 0; i < N; ++i) {
    volatile bool val = testXFastTrie.contains(keys[i]);
  }
  end = std::chrono::high_resolution_clock::now();
  std::cout << "X-fast trie contains: " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count()
            << "ms" << std::endl;

  begin = std::chrono::high_resolution_clock::now();
  for (int i = 0; i < N; ++i) {
    volatile bool val = testM.contains(keys[i]);
  }
  end = std::chrono::high_resolution_clock::now();
  std::cout << "Set contains: " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() << "ms\n"
            << std::endl;

  // -----------------------------------------------------------------------------------------------------------------

  begin = std::chrono::high_resolution_clock::now();
  for (int i = 0; i < N; ++i) {
    volatile auto val = testXFastTrie.successor(values[i]);
  }
  end = std::chrono::high_resolution_clock::now();
  std::cout << "X-fast trie successor: " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count()
            << "ms" << std::endl;

  begin = std::chrono::high_resolution_clock::now();
  for (int i = 0; i < N; ++i) {
    volatile auto val = testM.find(values[i]);
  }
  end = std::chrono::high_resolution_clock::now();
  std::cout << "Set successor: " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() << "ms\n"
            << std::endl;

  // -----------------------------------------------------------------------------------------------------------------

  begin = std::chrono::high_resolution_clock::now();
  for (int i = 0; i < N; ++i) {
    testXFastTrie.erase(keys[i]);
  }
  end = std::chrono::high_resolution_clock::now();
  std::cout << "X-fast trie erase: " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count()
            << "ms" << std::endl;

  begin = std::chrono::high_resolution_clock::now();
  for (int i = 0; i < N; ++i) {
    testM.erase(keys[i]);
  }
  end = std::chrono::high_resolution_clock::now();
  std::cout << "Set erase: " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() << "ms\n"
            << std::endl;

}


int main() {
  runTests();
  runPerfTests();

  return 0;
}
