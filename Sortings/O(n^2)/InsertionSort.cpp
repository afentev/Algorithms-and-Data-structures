#include <iostream>
#include <vector>
#include <chrono>

template <typename T>
void insertionSort(std::vector<T>& v) {
  for (size_t i = 1; i < v.size(); ++i) {
    for (int j = i - 1; j >= 0 && v[j] > v[j + 1]; --j) {
      std::swap(v[j], v[j + 1]);
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
  insertionSort(v);
  auto end = std::chrono::high_resolution_clock::now();
  std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(end-begin).count() << "ms" << std::endl;

  std::vector<int> copy = v;
  std::sort(copy.begin(), copy.end());
  std::cout << (copy == v) << std::endl;
  return 0;
}