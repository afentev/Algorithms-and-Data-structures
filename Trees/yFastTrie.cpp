#include <iostream>
#include <vector>
#include <chrono>
#include <set>
#include <utility>
#include <list>
#include <cassert>
#include <unordered_map>

#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/tree_policy.hpp>

template <size_t w, typename Val>
class yFastTrie;

template <typename Val>
using tree = __gnu_pbds::tree<Val,
                              __gnu_pbds::null_type,
                              std::less<>,
                              __gnu_pbds::splay_tree_tag,
                              __gnu_pbds::null_node_update>;
//template <typename Val>
//using tree = std::set<Val>;  // also possible but slower

template <typename Val>
struct Node {
  using listIter = typename std::list<std::pair<Val, tree<Val>>>::iterator;

  Node() = default;
  Node(const listIter & v): max(v), min(v) {}

  listIter min;
  listIter max;
};

template <size_t w, typename Val>
class xFastTrie {
  friend class yFastTrie<w, Val>;

 private:
  using listIter = typename Node<Val>::listIter;
  using Container = __gnu_pbds::gp_hash_table<Val, Node<Val>>;
//  using Container = std::unordered_map<Val, Node<Val>>;  // also possible but slower

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

  listIter successorIter(const Val& v) {
    int level = getFastPrefix(v);

    if (level == w - 1) {
      return mapper[level][v].min;  // leaf
    } else if (level == -1) {
      return std::prev(list.end());
    }
    Val prefix = v >> (w - level - 2);
    if (prefix & 1) {
      return std::next(mapper[level][v >> (w - level - 1)].max);
    } else {
      return mapper[level][v >> (w - level - 1)].min;
    }
  }

 public:
  xFastTrie(const Val& negInf, const Val& posInf) {
    mapper.resize(w);
    list.emplace_back(negInf, tree<Val> ());
    list.emplace_back(posInf, tree<Val> ());
  }

  listIter insert(const Val& v, const tree<Val>& subtree) {
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
    it = list.insert(it, std::make_pair(v, subtree));

    // step down and insert
    for (int levelDown = level; levelDown < w; ++levelDown) {
      mapper[levelDown][v >> (w - levelDown - 1)] = Node<Val>(it);
    }

    // step up and update min and max
    for (int levelUp = level; levelUp >= 0; --levelUp) {
      listIter& min = mapper[levelUp][cur].min;
      listIter& max = mapper[levelUp][cur].max;
      if (v < min->first) {
        min = it;
      } else if (v > max->first) {
        max = it;
      }
      cur >>= 1;
    }
    return it;
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
    return successorIter(v)->first;
  }

  Val predecessor(const Val& v) {
    int level = getFastPrefix(v);

    if (level == w - 1) {
      return v;
    }
    Val prefix = v >> (w - level - 2);
    if (prefix & 1) {
      return mapper[level][v >> (w - level - 1)].max->first;
    } else {
      return std::prev(mapper[level][v >> (w - level - 1)].min)->first;
    }
  }

  bool contains(const Val& v) {
    return mapper[w - 1].find(v) != mapper[w - 1].end();
  }

 private:
  std::vector<Container> mapper;
  std::list<std::pair<Val, tree<Val>>> list;
};

template <size_t w, typename Val>
class yFastTrie {
  using listIter = typename xFastTrie<w, Val>::listIter;

 private:
  xFastTrie<w, Val> trie;
  size_t size = 0;

  void splitTrees(const listIter& node) {
    auto medianIt = node->second.begin();
    std::advance(medianIt, w);
    auto left = tree<Val>(node->second.begin(), medianIt);
    auto right = tree<Val>(medianIt, node->second.end());
    trie.erase(node->first);
    trie.insert(*left.rbegin(), left);
    trie.insert(*right.rbegin(), right);
  }

  std::pair<listIter, Val> successorIter(const Val& v) {
    listIter tree = trie.successorIter(v);
    listIter treePrev = std::prev(tree);
    auto right = tree->second.lower_bound(v);
    auto left = treePrev->second.lower_bound(v);
    if (right == tree->second.end()) {
      if (left == treePrev->second.end()) {
        return {treePrev, trie.list.rbegin()->first};
      }
      return {treePrev, *left};
    } else if (left == treePrev->second.end()) {
      return {tree, *right};
    } else {
      if (*left < *right) {
        return {treePrev, *left};
      } else {
        return {tree, *right};
      }
    }
  }

 public:
  yFastTrie(const Val& negInf, const Val& posInf): trie(negInf, posInf) {}

  void insert(const Val& v) {
    auto [node, val] = successorIter(v);
    if (size == 0) {
      tree<Val> x;
      x.insert(v);
      trie.insert(v, x);
      node = std::next(trie.list.begin());
    } else {
      if (node == std::prev(trie.list.end())) {
        node = std::prev(node);
      }
      node->second.insert(v);
    }
    if (node->second.size() == 2 * w + 1) {
      splitTrees(node);
    }
    size++;
  }

  void erase(const Val& v) {
    auto [node, _] = successorIter(v);
    if (size == 1) {
      trie.erase(v);
      size = 0;
      return;
    }
    if (node->first == v) {
      tree<Val> tmp = node->second;
      trie.erase(v);
      if (v == *tmp.begin()) {
        node = trie.insert(*std::prev(tmp.end()), tmp);
      } else {
        node = trie.insert(*tmp.begin(), tmp);
      }
    }
    node->second.erase(v);
    listIter neighbor;
    if (std::next(node) == std::prev(trie.list.end())) {
      neighbor = std::prev(node);
    } else {
      neighbor = std::next(node);
    }
    if (size > 2 * w) {
      if (node->second.size() == w / 2 - 1) {
        for (auto x: node->second) {
          neighbor->second.insert(x);
        }

        trie.erase(node->first);
        if (neighbor->second.size() >= 2 * w + 1) {
          splitTrees(neighbor);
        }
      }
    }
    --size;
  }

  Val successor(const Val& v) {
    return successorIter(v).second;
  }

  Val predecessor(const Val& v) {
    listIter tree = trie.successorIter(v);
    listIter treePrev = std::prev(tree);
    auto right = tree->second.lower_bound(v);
    auto left = treePrev->second.lower_bound(v);
    if ((left != treePrev->second.end() && *left == v) || (right != tree->second.end() && *right == v)) {
      return v;
    }
    if (left == treePrev->second.begin()) {
      if (right == tree->second.begin()) {
        return trie.list.begin()->first;
      }
      return *std::prev(right);
    } else if (right == tree->second.begin()) {
      return *std::prev(left);
    }
    return std::max(*std::prev(left), *std::prev(right));
  }

  bool contains(const Val& v) {
    auto it = successorIter(v).first;
    auto prev = std::prev(it);
    return it->second.find(v) != it->second.end() || prev->second.find(v) != prev->second.end();
  }
};

void runTests() {
  yFastTrie<32, uint32_t> t(0, UINT32_MAX);
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
  int k = 0;
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
  const int N = 1e5;
  yFastTrie<32, uint32_t> testYFastTrie(0, UINT32_MAX);
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
    testYFastTrie.insert(keys[i]);
  }
  auto end = std::chrono::high_resolution_clock::now();
  std::cout << "Y-fast trie insertion: " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count()
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
    volatile bool val = testYFastTrie.contains(keys[i]);
  }
  end = std::chrono::high_resolution_clock::now();
  std::cout << "Y-fast trie contains: " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count()
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
    volatile auto val = testYFastTrie.successor(values[i]);
  }
  end = std::chrono::high_resolution_clock::now();
  std::cout << "Y-fast trie successor: " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count()
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
    testYFastTrie.erase(keys[i]);
  }
  end = std::chrono::high_resolution_clock::now();
  std::cout << "Y-fast trie erase: " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count()
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
}
