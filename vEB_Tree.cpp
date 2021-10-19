//
// Created by afentev on 10/14/21.
//

#include <iostream>
#include <cmath>
#include <vector>
#include <random>
#include <chrono>

// this current implementation supports only 32-bit numbers (it seems easy to be changed)
class vEB {
 public:
  explicit vEB(uint8_t sz) {  // Receives k from [0; 2^k)
    deactivateNode();
    if (sz > 6) {  // if size < 64 store numbers in bitset (uint64_t)
	  uint8_t a = 6;
	  while (2 * a < sz) {
		a <<= 1;
	  }
	  /*
	   * a is chosen that way to achieve the most effective Nodes structure alignment
	   * with that algorithm all nodes will perfectly converge on low-depth into bitsets which size is 2^6=64
	   * this is done with 2 purposes: firstly, this approach uses less memory than taking the square roots method or
	   * simply bit shifts. the problem with these 2 methods is a big number of sparse nodes and weak efficiency in
	   * convergence into bitsets. secondly, reducing the number of nodes in tree speeds up tree construction
	   * ~5 times (very approximately).
	   */
	  uint32_t l1 = 1 << a;
	  of2 = sz - a;
	  l2 = 1u << of2;
	  mask = l2 - 1;

	  children.reserve(l1);
	  for (int i = 0; i < l1; ++i) {
		children.emplace_back(of2);
	  }
	  used = new vEB(a);
    } else {
	  l2 = 0ull;
	  of2 = 3;  // this variable will be used in the program as a flag (size < 64)
	  used = nullptr;
	}
  }

  void insert(int32_t el) {
	if (of2 <= 3) {
	  l2 |= 1ull << (63 - el);
	  return;
	}
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
	if (of2 <= 3) {
	  l2 &= ~(1ull << (63 - el));
	  return;
	}
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
	if (of2 <= 3) {
	  uint64_t tmp = (l2 << el);
	  if (tmp == 0) {
		return -1;
	  }
	  return el + __builtin_clzll(tmp);
	}
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
	  if (attempt >= index) {  // we could find next in this subtree, do that
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
	if (of2 <= 3) {
	  uint64_t tmp = (l2 >> (63 - el));
	  if (tmp == 0) {
		return -1;
	  }
	  return el - __builtin_ctzll(tmp);
	}
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
	  if (attempt <= index) {  // we could find prev in this subtree, do that
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

  [[nodiscard]] int getMin() const {
	if (of2 <= 3) {
	  return __builtin_clzll(l2);
	}
	return min;
  }

  [[nodiscard]] int getMax() const {
	if (of2 <= 3) {
	  return 63 - __builtin_ctzll(l2);
	}
	return max;
  }

  [[nodiscard]] bool isEmpty() const {
	if (of2 <= 3) {
	  return l2 == 0;
	}
    return min == -1 && max == -2;
  }

 private:
  [[nodiscard]] uint32_t hi(uint32_t key) const {
	return key >> of2;  // left half of the bits
  }

  [[nodiscard]] uint32_t lo(uint32_t key) const {
	return key & mask;  // right half of the bits
  }

  void deactivateNode() {
	if (of2 <= 3) {
	  l2 = 0;
	}
	min = -1;
	max = -2;
  }

  bool isUsedEmpty() {
	return used == nullptr || used->isEmpty();
  }

  int32_t min;
  int32_t max;
  uint64_t l2;  // ambiguous meaning of l2!
  /*
   * when of2 <= 3 (size <= 64) it works like bitset and stores elements;
   * otherwise it stores O(√size) - look at the constructor for details;
   * this is done with the purpose of reducing memory space and not using 2 variables
   */
  uint8_t of2;  // fast lo(x)
  uint32_t mask;  // fast hi(x)
  std::vector<vEB> children;
  vEB* used;
};

int main() {
  int n = 1e7;
  const int max_K = 30;  // [0; 2^30)

  std::random_device rd;
  std::mt19937 g(rd());
  std::vector<int> x(n);

  for (int i = 0; i < n; ++i) {
	x[i] = i;
  }
  std::shuffle(x.begin(), x.end(), g);

  std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

  vEB t(max_K);

  std::chrono::steady_clock::time_point allocate = std::chrono::steady_clock::now();
  std::cout << "Allocation = " << std::chrono::duration_cast<std::chrono::milliseconds> (allocate - begin).count() << " [ms]" << std::endl;

  for (int i = 0; i < n; ++i) {
	t.insert(x[i]);
  }

  std::chrono::steady_clock::time_point insert = std::chrono::steady_clock::now();
  std::cout << "Insertion  = " << std::chrono::duration_cast<std::chrono::milliseconds> (insert - allocate).count() << " [ms]" << std::endl;

  for (int i = 0; i < n; ++i) {
	t.remove(t.getMin());
  }

  std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
  std::cout << "Remove     = " << std::chrono::duration_cast<std::chrono::milliseconds> (end - insert).count() << " [ms]" << std::endl;
  std::cout << "Total      = " << std::chrono::duration_cast<std::chrono::milliseconds> (end - begin).count() << " [ms]" << std::endl;

  return 0;
}