//
// Created by afentev on 9/28/21.
//

#include <iostream>
#include <vector>
#include <chrono>
#include <random>

template<class TKey, class TVal>
class BinaryHeap {
 public:
  void insert(TKey k, TVal v) {
    keys.push_back(k);
    values.push_back(v);
    siftUp(end++);
  }

  TVal extractMin() {
    TVal ret = values[0];
    values[0] = values[--end];
    keys[0] = keys[end];
    siftDown(0);
    values.pop_back();
    keys.pop_back();
    return ret;
  }

  void reserve(size_t size) {
    values.reserve(size);
    keys.reserve(size);
  }

  void siftDown(int index) {
    while (2 * index + 1 < end) {
      int left = 2 * index + 1;
      int right = 2 * index + 2;
      int j = left;
      if (right < end && keys[right] < keys[left]) {
        j = right;
      }
      if (keys[index] <= keys[j]) {
        break;
      }
      std::swap(keys[index], keys[j]);
      std::swap(values[index], values[j]);
      index = j;
    }
  }

  void siftUp(int index) {
    while (keys[index] < keys[(index - 1) / 2]) {
      std::swap(keys[index], keys[(index - 1) / 2]);
      std::swap(values[index], values[(index - 1) / 2]);
      index = (index - 1) / 2;
    }
  }

  TVal getMin() {
    return values[0];
  }

  int size() {
    return end;
  }

 private:
  int end = 0;
  std::vector<TVal> values;
  std::vector<TVal> keys;
};

int main() {
  int n = 1e6;

  std::random_device rd;
  std::mt19937 g(rd());
  std::vector<int> x(n);

  for (int i = 0; i < n; ++i) {
    x[i] = i;
  }

  BinaryHeap<int, int> t;
  t.reserve(n);

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
