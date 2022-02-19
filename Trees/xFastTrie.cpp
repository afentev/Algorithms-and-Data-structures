#include <iostream>
#include <vector>
#include <chrono>
#include <set>
#include <list>
#include <unordered_map>

#include <ext/pb_ds/assoc_container.hpp>

template <typename Val>
struct Node {
  using listIter = typename std::list<Val>::iterator;

  Node() = default;
  Node(const listIter & v): max(v), min(v) {}

  listIter min;
  listIter max;
};

template <size_t w, typename Val>
class xFastTrie {
  using Container = __gnu_pbds::gp_hash_table<Val, Node<Val>>;
//  using Container = std::unordered_map<Val, Node<Val>>;  // also possible but slower
  using listIter = typename Node<Val>::listIter;

 private:
  int getFastPrefix(const Val& v) {
    int lhs = -1;
    int rhs = w;
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
  xFastTrie(const Val& negInf, const Val& posInf) {
    mapper.resize(w);
    list.push_back(negInf);
    list.push_back(posInf);
  }

  void insert(const Val& v) {
    listIter it;
    int level = getFastPrefix(v) + 1;
    Val cur = v >> (w - level - 1);
    if (level == 0) {
      if (cur & 1) {
        it = std::prev(list.end());
      } else {
        it = std::next(list.begin());
      }
    } else if (cur & 1) {
      it = std::next(mapper[level - 1][cur >> 1].max);
    } else {
      it = mapper[level - 1][cur >> 1].min;
    }
    it = list.insert(it, v);

    // step down and insert
    for (int levelDown = level; levelDown < w; ++levelDown) {
      mapper[levelDown][v >> (w - levelDown - 1)] = Node<Val>(it);
    }

    // step up and update min and max
    for (int levelUp = level; levelUp >= 0; --levelUp) {
      listIter& min = mapper[levelUp][cur].min;
      listIter& max = mapper[levelUp][cur].max;
      if (v < *min) {
        min = it;
      } else if (v > *max) {
        max = it;
      }
      cur >>= 1;
    }
  }

  void erase(const Val& v) {
    listIter it = mapper[w - 1][v].min;  // node in list
    listIter suc = std::next(it);
    listIter pred = std::prev(it);

    for (int level = w - 1; level >= 0; --level) {
      Val cur = v >> (w - level - 1);
      Node<Val>& node = mapper[level][cur];
      if (node.min == node.max) {
        mapper[level].erase(cur);
      } else if (node.min == it) {
        node.min = suc;
      } else if (node.max == it) {
        node.max = pred;
      }
    }
    list.erase(it);
  }

  Val successor(const Val& v) {
    int level = getFastPrefix(v);

    if (level == w - 1) {
      return v;
    }
    Val prefix = v >> (w - level - 2);
    if (prefix & 1) {
      return *std::next(mapper[level][v >> (w - level - 1)].max);
    } else {
      return *mapper[level][v >> (w - level - 1)].min;
    }
  }

  Val predecessor(const Val& v) {
    int level = getFastPrefix(v);

    if (level == w - 1) {
      return v;
    }
    Val prefix = v >> (w - level - 2);
    if (prefix & 1) {
      return *mapper[level][v >> (w - level - 1)].max;
    } else {
      return *std::prev(mapper[level][v >> (w - level - 1)].min);
    }
  }

  bool contains(const Val& v) {
    return mapper[w - 1].find(v) != mapper[w - 1].end();
  }

  std::vector<Container> mapper;
  std::list<Val> list;
};

void runTests() {
  xFastTrie<32, uint32_t> t(0, UINT32_MAX);
  std::set<int> m;

  int N = 1e5;
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
  for (auto x: r) {
    auto it = m.lower_bound(x);
    uint32_t pred, suc;
    if (it == m.begin()) {
      pred = 0;
      suc = *it;
    } else if (it == m.end()) {
      suc = UINT32_MAX;
      pred = *std::prev(it);
    } else {
      pred = *std::prev(it);
      suc = *it;
    }
    if (x == *it) {
      pred = x;
      suc = x;
    }
    assert(suc == t.successor(x));
    assert(pred == t.predecessor(x));
  }

  std::cout << "All tests passed!\n" << std::endl;
}


void runPerfTests() {
  const int N = 1e6;
  xFastTrie<32, uint32_t> testXFastTrie(0, UINT32_MAX);
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
