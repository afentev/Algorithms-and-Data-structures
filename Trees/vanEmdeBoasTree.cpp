//
// Created by afentev on 10/14/21.
//

#include <iostream>
#include <vector>
#include <chrono>
#include <set>


// this constexpr are needed for const template declarations   ¯\_(ツ)_/¯
constexpr static uint8_t const aSize[33] {3, 3, 3, 3, 3, 3, 3, 6, 6, 6, 6, 6, 6, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12,
                                          12, 12, 24, 24, 24, 24, 24, 24, 24, 24};
constexpr const uint8_t* aPtr = &aSize[0];

constexpr static uint8_t const ofSize[33] {3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 4, 5, 6, 3, 3, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12,
                                           3, 3, 3, 4, 5, 6, 7, 8};
constexpr const uint8_t* ofPtr = &ofSize[0];


// this current implementation supports only 32-bit numbers (it seems easy to be changed)
//#pragma pack(1)

template <uint8_t sz>
class vEB {
 public:
  explicit vEB(): min(-1), max(-2), of2(ofPtr[sz]), l2(1u << of2) {  // Receives k from [0; 2^k)
    uint8_t a = aSize[sz];
    /*
     * uint8_t a = 6;
     * while (2 * a < sz) {
     *   a <<= 1;
     * }
     *
     * a is chosen that way to achieve the most effective Nodes structure alignment
     * with that algorithm all nodes will perfectly converge on low-depth into bitsets which size is 2^6=64
     * this is done with 2 purposes: firstly, this approach uses less memory than taking the square roots method or
     * simply bit shifts. the problem with these 2 methods is a big number of sparse nodes and weak efficiency in
     * convergence into bitsets. secondly, reducing the number of nodes in vEB speeds up vEB construction
     * ~5 times (very approximately).
     */
    uint32_t l1 = 1 << a;
    //of2 = sz - a;        ofSize contains right these values
//    of2 = ofPtr[sz];
//    l2 = 1u << of2;

    children.reserve(l1);
    for (int i = 0; i < l1; ++i) {
      children.push_back(vEB<*(ofPtr + sz)>());
    }
    used = new vEB<*(aPtr + sz)>;
  }

  void insert(int32_t el) {
    if (isEmpty()) {
      min = max = el;
      return;
    }
    bool f = min == max;
    if (el < min) {  // updated minimum and insert previous one in the children
      std::swap(el, min);
    }
    if (el > max) {  // updated maximum and insert previous one in the children
      std::swap(el, max);
    }
    if (f) {  // if no children update is needed (size == 2)
      return;
    }
    uint32_t child = hi(el);
    uint32_t index = lo(el);
    children[child].insert(index);  // insert element in the children array
    if (children[child].getMin() == children[child].getMax()) {
      used->insert(child);  // insert the child into the used array if it was not inserted
    }
  }

  void remove(int32_t el) {
    if (isEmpty()) return;
    if (min == max) {
      deactivateNode();
      return;
    }
    if (used == nullptr) {  // (size == 2)
      if (el == 1) {
        if (min == 1) {
          deactivateNode();
        } else if (min == 0) {  // always true
          max = 0;
        }
      } else {
        if (max == 0) {
          deactivateNode();
        } else if (max == 1) {  // always true
          min = 1;
        }
      }
      return;  // trivial case; no additional actions are needed
    }
    if (el == min) {
      if (isUsedEmpty()) {
        min = max;
        return;
      }
      /*
       * forget (remove) previous minimum;
       * update minimum variable with the next minimum (minimum in minimal non-empty child)
       * recursively remove new minimum from children
       */
      el = min = l2 * used->getMin() + children[used->getMin()].getMin();
    }
    if (el == max) {
      if (isUsedEmpty()) {
        max = min;
        return;
      }
      /*
       * forget (remove) previous maximum;
       * update maximum variable with the next maximum (maximum in maximal non-empty child)
       * recursively remove new maximum from children
       */
      el = max = l2 * used->getMax() + children[used->getMax()].getMax();
    }

    uint32_t child = hi(el);
    uint32_t index = lo(el);
    children[child].remove(index);
    if (children[child].isEmpty()) {
      used->remove(child);  // remove child from used array if it became empty after deletion
    }
  }

  int32_t next(int32_t el) {
    if (el > max || isEmpty()) {
      return -1;
    }
    if (el <= min) {
      return min;
    }
    if (isUsedEmpty()) {
      return max;
    }
    uint32_t child = hi(el);
    uint32_t index = lo(el);

    if (!children[child].isEmpty()) {
      int32_t attempt = children[child].getMax();
      if (attempt >= index) {  // we could find next in this subvEB, do that
        int32_t res = children[child].next(index);
        return res + l2 * child;
      }
    }
    int32_t bucket = used->next(child + 1);  // recursively find the child that contains "next" element
    if (bucket == -1) {  // no next in children, return max
      return max;
    }
    int32_t res = children[bucket].getMin();  // getting the minimal element from the first suitable child
    return res + l2 * bucket;
  }

  int32_t prev(int32_t el) {
    if (min > el || isEmpty()) {
      return -1;
    }
    if (el >= max) {
      return max;
    }
    if (isUsedEmpty()) {
      return min;
    }
    uint32_t child = hi(el);
    uint32_t index = lo(el);

    if (!children[child].isEmpty()) {
      int32_t attempt = children[child].getMin();
      if (attempt <= index) {  // we could find prev in this subvEB, do that
        int32_t res = children[child].prev(index);
        return res + l2 * child;
      }
    }
    int32_t bucket = used->prev(child - 1);  // recursively find the child that contains "prev" element
    if (bucket == -1) {  // no prev in children, return min
      return min;
    }
    int32_t res = children[bucket].getMax();  // getting the maximal element from the first suitable child
    return res + l2 * bucket;
  }

  [[nodiscard]] int32_t getMin() const {
    return min;
  }

  [[nodiscard]] int32_t getMax() const {
    return max;
  }

  [[nodiscard]] bool isEmpty() const {
    return min == -1 && max == -2;
  }

 private:
  [[nodiscard]] uint32_t hi(uint32_t key) const {
    return key >> of2;  // left half of the bits
  }

  [[nodiscard]] uint32_t lo(uint32_t key) const {
    return key & (l2 - 1);  // right half of the bits
  }

  void deactivateNode() {
    min = -1;
    max = -2;
  }

  bool isUsedEmpty() {
    return used == nullptr || used->isEmpty();
  }

  std::vector<vEB<*(ofPtr + sz)>> children;
  vEB<*(aPtr + sz)>* used;
  int32_t min;
  int32_t max;
  const uint8_t of2;  // fast lo(x) and hi(x)
  const uint64_t l2;
};

template <>  // 2^6=64 - elements are in bitset
class vEB<6> {
 public:
  vEB() {
    bitset = 0ull;
  }

  void insert(int32_t el) {
    bitset |= 1ull << (63 - el);
  }

  void remove(int32_t el) {
    bitset &= ~(1ull << (63 - el));
  }

  [[nodiscard]] int32_t next(int32_t el) const {
    uint64_t tmp = (bitset << el);
    if (tmp == 0 || el == 64) {
      return -1;
    }
    return el + __builtin_clzll(tmp);
  }

  [[nodiscard]] int32_t prev(int32_t el) const {
    uint64_t tmp = (bitset >> (63 - el));
    if (tmp == 0 || el == -1) {
      return -1;
    }
    return el - __builtin_ctzll(tmp);
  }

  [[nodiscard]] int32_t getMin() const {
    return __builtin_clzll(bitset);
  }

  [[nodiscard]] int32_t getMax() const {
    return 63 - __builtin_ctzll(bitset);
  }

  [[nodiscard]] bool isEmpty() const {
    return bitset == 0;
  }

 private:
  uint64_t bitset;
};

template <>
class vEB<5> {
 public:
  vEB() {
    bitset = 0u;
  }

  void insert(int32_t el) {
    bitset |= 1u << (31 - el);
  }

  void remove(int32_t el) {
    bitset &= ~(1u << (31 - el));
  }

  [[nodiscard]] int32_t next(int32_t el) const {
    uint64_t tmp = (bitset << el);
    if (tmp == 0 || el == 32) {
      return -1;
    }
    return el + __builtin_clz(tmp);
  }

  [[nodiscard]] int32_t prev(int32_t el) const {
    uint64_t tmp = (bitset >> (31 - el));
    if (tmp == 0 || el == -1) {
      return -1;
    }
    return el - __builtin_ctz(tmp);
  }

  [[nodiscard]] int32_t getMin() const {
    return __builtin_clz(bitset);
  }

  [[nodiscard]] int32_t getMax() const {
    return 31 - __builtin_ctz(bitset);
  }

  [[nodiscard]] bool isEmpty() const {
    return bitset == 0;
  }

 private:
  uint32_t bitset;
};

template <>
class vEB<4> {
 public:
  vEB() {
    bitset = 0u;
  }

  void insert(int32_t el) {
    bitset |= 1u << (15 - el);
  }

  void remove(int32_t el) {
    bitset &= ~(1u << (15 - el));
  }

  [[nodiscard]] int32_t next(int32_t el) const {
    uint32_t tmp = (bitset << (el + 16));
    if (tmp == 0 || el == 16) {
      return -1;
    }
    return el + __builtin_clz(tmp);
  }

  [[nodiscard]] int32_t prev(int32_t el) const {
    uint32_t tmp = (bitset >> (15 - el));
    if (tmp == 0 || el == -1) {
      return -1;
    }
    return el - __builtin_ctz(tmp);
  }

  [[nodiscard]] int32_t getMin() const {
    return __builtin_clz(bitset) - 16;
  }

  [[nodiscard]] int32_t getMax() const {
    return 15 - __builtin_ctz(bitset);
  }

  [[nodiscard]] bool isEmpty() const {
    return bitset == 0;
  }

 private:
  uint16_t bitset;
};

template <>
class vEB<3> {
 public:
  vEB() {
    bitset = 0u;
  }

  void insert(int32_t el) {
    bitset |= 1u << (7 - el);
  }

  void remove(int32_t el) {
    bitset &= ~(1u << (7 - el));
  }

  [[nodiscard]] int32_t next(int32_t el) const {
    uint32_t tmp = (bitset << (el + 24));
    if (tmp == 0 || el == 8) {
      return -1;
    }
    return el + __builtin_clz(tmp);
  }

  [[nodiscard]] int32_t prev(int32_t el) const {
    uint32_t tmp = (bitset >> (7 - el));
    if (tmp == 0 || el == -1) {
      return -1;
    }
    return el - __builtin_ctz(tmp);
  }

  [[nodiscard]] int32_t getMin() const {
    return __builtin_clz(bitset) - 24;
  }

  [[nodiscard]] int32_t getMax() const {
    return 7 - __builtin_ctz(bitset);
  }

  [[nodiscard]] bool isEmpty() const {
    return bitset == 0;
  }

 private:
  uint8_t bitset;
};

#include <cassert>

void runTests() {
  vEB<30> t;
  std::set<int> m;

  int N = 1e5;
  std::vector<int> d, r;
  d.reserve(N);

  for (int i = 0; i < N; ++i) {
    int k = rand() % (1ull << 30);
    int v = rand() % (1ull << 30);
    t.insert(k);
    m.insert(k);
    r.push_back(k);
  }

  for (auto x: r) {
    assert((t.next(x) == x) == m.contains(x));
    if (rand() % 2 == 1) {
      d.push_back(x);
    }
  }

  for (int x: d) {
    m.erase(x);
    t.remove(x);
  }

  for (auto x: r) {
    assert(m.contains(x) == (t.next(x) == x));
  }

  for (auto x: r) {
    auto it = m.lower_bound(x);
    uint32_t pred, suc;
    if (it == m.begin()) {
      pred = -1;
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
    assert(suc == t.next(x));
    assert(pred == t.prev(x));
  }

  std::cout << "All tests passed!\n" << std::endl;
}


void runPerfTests() {
  srand(42);
  int N = 1e6;

  std::chrono::high_resolution_clock::time_point begin = std::chrono::high_resolution_clock::now();
  vEB<30> vEB;
  std::chrono::high_resolution_clock::time_point end = std::chrono::high_resolution_clock::now();

  std::cout << "vEB<30> allocation: " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count()
            << "ms" << std::endl;

  begin = std::chrono::high_resolution_clock::now();
  std::set<int> testM;
  end = std::chrono::high_resolution_clock::now();
  std::cout << "set allocation: " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count()
            << "ms\n" << std::endl;

  std::vector<int> keys, values;
  keys.reserve(N);
  values.reserve(N);

  std::set<int> unique;
  for (int i = 0; i < N; ++i) {
    unique.insert(rand() % ((1 << 30) - 1));
  }
  keys = std::vector(unique.begin(), unique.end());
  N = keys.size();

  // -----------------------------------------------------------------------------------------------------------------
  begin = std::chrono::high_resolution_clock::now();
  for (int i = 0; i < N; ++i) {
    vEB.insert(keys[i]);
  }
  end = std::chrono::high_resolution_clock::now();
  std::cout << "vEB<30> insertion: " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count()
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
    volatile bool val = (vEB.next(keys[i]) == keys[i]);
  }
  end = std::chrono::high_resolution_clock::now();
  std::cout << "vEB<30> contains: " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count()
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
    volatile auto val = vEB.next(values[i]);
  }
  end = std::chrono::high_resolution_clock::now();
  std::cout << "vEB<30> successor: " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count()
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
    vEB.remove(keys[i]);
  }
  end = std::chrono::high_resolution_clock::now();
  std::cout << "vEB<30> erase: " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count()
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
