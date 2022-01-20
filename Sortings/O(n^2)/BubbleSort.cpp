#include <iostream>
#include <vector>
#include <chrono>

template <typename T>
void bubbleSort(std::vector<T>& v) {
  bool isSorted = false;
  for (size_t i = 0; i < v.size() && !isSorted; ++i) {
    isSorted = true;
    for (size_t j = 0; j < v.size() - i - 1; ++j) {
      if (v[j] > v[j + 1]) {
        isSorted = false;
        std::swap(v[j], v[j + 1]);
      }
    }
  }
}

int main() {
  std::vector<int> v;
  const int N = 500;
  for (int i = 0; i < N; ++i) {
    v.push_back(rand() % INT32_MAX);
  }

  auto begin = std::chrono::high_resolution_clock::now();
  bubbleSort(v);
  auto end = std::chrono::high_resolution_clock::now();
  std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(end-begin).count() << "ms" << std::endl;

  std::vector<int> copy = v;
  std::sort(copy.begin(), copy.end());
  std::cout << (copy == v) << std::endl;
  return 0;
}