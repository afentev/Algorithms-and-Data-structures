#include <iostream>
#include <algorithm>
#include <stack>
#include <list>
#include <queue>
#include <vector>
#include <chrono>
#include <random>

int inf = 1e9;

template<class TKey, class TVal>
class BinomialHeap;

template<class TKey, class TVal>
class BinomialHeapImpl_;

template<class TKey, class TVal>
class BinomialTree_;

template<class TKey, class TVal>
class BinomialTree_ {
 public:
  BinomialTree_() {
	initialized = false;
  }

  BinomialHeapImpl_<TKey, TVal>* merge(BinomialHeapImpl_<TKey, TVal>* t1, BinomialTree_<TKey, TVal>* t2, bool deleted = false) {
	if (!t1->initialized) {
	  auto* res = new BinomialHeapImpl_<TKey, TVal>;
	  res->initialized = true;
	  if (!deleted) {
		res->heap.push_back(t2);
	  } else {
		res->heap = t2->heap;
	  }
	  return res;
	}
	if (!deleted) {
	  t1->heap.push_back(t2);
	  t1->initialized = true;
	  return t1;
	}
	auto* res = new BinomialHeapImpl_<TKey, TVal>;
	while (!t1->heap.empty() && !t2->heap.empty()) {
	  if (t1->heap.front()->degree < t2->heap.front()->degree) {
		res->heap.push_back(t1->heap.front());
		t1->heap.pop_front();
	  } else {
		res->heap.push_back(t2->heap.front());
		t2->heap.pop_front();
	  }
	}
	res->initialized = true;

// for deque:
//        while (!t1->heap.empty()) {
//            res->heap.push_back(t1->heap.front());
//            t1->heap.pop_front();
//        }
//        while (!t2->heap.empty()) {
//            res->heap.push_back(t2->heap.front());
//            t2->heap.pop_front();
//        }

	// for list:
	res->heap.splice(res->heap.end(), t1->heap);
	res->heap.splice(res->heap.end(), t2->heap);

	auto iterator = res->heap.begin();
	while (iterator != res->heap.end() && next(iterator) != res->heap.end()) {
	  if ((*iterator)->degree == (*next(iterator))->degree) {
		if ((*iterator)->key > (*next(iterator))->key) {
		  (*next(iterator))->heap.push_back(*iterator);
		  iterator = res->heap.erase(iterator);
		} else {
		  (*iterator)->heap.push_back(*next(iterator));
		  res->heap.erase(next(iterator));
		}
		(*iterator)->degree++;
	  } else if ((*iterator)->degree > (*next(iterator))->degree) {
		std::swap(*iterator, *next(iterator));
		iterator++;
	  } else {
		iterator++;
	  }
	}

	delete t2;
	return res;
  }

  TKey key;
  TVal value;
  int degree;
  bool initialized;
  std::list<BinomialTree_<TKey, TVal>*> heap;
};

template<class TKey, class TVal>
class BinomialHeapImpl_ {
 public:
  friend class BinomialHeap<TKey, TVal>;
  friend class BinomialTree_<TKey, TVal>;

  BinomialHeapImpl_() {
	degree = 0;
	initialized = false;
  }

  void decreaseKey(BinomialHeapImpl_<TKey, TVal>* node, TKey newKey) {   /* todo */   }

 private:
  BinomialTree_<TKey, TVal>* minimaPtr() {
	BinomialTree_<TKey, TVal>* minima = heap.front();
	for (auto it = heap.begin(); it != heap.end(); ++it) {
	  if ((*it)->key < minima->key) {
		minima = *it;
	  }
	}
	return minima;
  }
  int degree;
  bool initialized;
  std::list<BinomialTree_<TKey, TVal>*> heap;
};

template<class TKey, class TVal>
class BinomialHeap {
 public:
  BinomialHeap() {
	h = new BinomialHeapImpl_<TKey, TVal>;
  }

  void insert(TKey key_, TVal element) {
	s++;
	auto* H = new BinomialTree_<TKey, TVal>;
	auto* tree = new BinomialTree_<TKey, TVal>;
	H->heap.push_back(tree);
	H->initialized = true;

	tree->key = key_;
	tree->value = element;
	tree->degree = 1;
	tree->initialized = true;

	h = tree->merge(h, H, true);
	h->initialized = true;
  }

  TVal extractMin() {
	s--;
	auto* ptr = h->minimaPtr();

	if (h->heap.front() == ptr) {
	  if (h->heap.size() > 1) {
		h->heap.pop_front();
	  } else {
		h = new BinomialHeapImpl_<TKey, TVal>;
	  }
	} else {
	  auto it = h->heap.begin();
	  for (; *it != ptr && it != h->heap.end(); ++it) {}
	  h->heap.erase(it);
	}
	h = ptr->merge(h, ptr, true);
	if (h == nullptr) {
	  h = new BinomialHeapImpl_<TKey, TVal>;
	}
	TVal ret = ptr->value;
	return ret;
  }

  TVal getMin() {
	return h->minimaPtr()->value;
  }

  void remove(BinomialHeapImpl_<TKey, TVal>* node) {
	h->decreaseKey(node, -inf);
	h->extractMin();
  }

  bool empty() {
	return h->initialized;
  }

  int size() {
	return s;
  }

  int trees() {
	return h->heap.size();
  }

 private:
  BinomialHeapImpl_<TKey, TVal>* h;
  int s = 0;
};

int main() {
  int n = 1e5;

  std::random_device rd;
  std::mt19937 g(rd());
  std::vector<int> x(n);

  for (int i = 0; i < n; ++i) {
    x[i] = i;
  }

  BinomialHeap<int, int> t;

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
