#include <iostream>
#include <vector>
#include <chrono>

template <typename T>
void _mergeSort(std::vector<T>& v, size_t l, size_t r) {
  if (r - l == 1) {
    return;
  }
  size_t mid = (l + r) / 2;
  _mergeSort(v, l, mid);
  _mergeSort(v, mid, r);

  std::vector<T> tmp;
  tmp.reserve(r - l);
  size_t p1 = l;
  size_t p2 = mid;
  while (p1 < mid && p2 < r) {
    if (v[p1] < v[p2]) {
      tmp.push_back(v[p1]);
      ++p1;
    } else {
      tmp.push_back(v[p2]);
      ++p2;
    }
  }
  while (p1 < mid) {
    tmp.push_back(v[p1]);
    ++p1;
  }
  while (p2 < r) {
    tmp.push_back(v[p2]);
    ++p2;
  }
  for (size_t i = l; i < r; ++i) {
    v[i] = tmp[i - l];
  }
}
template <typename T>
void mergeSort(std::vector<T>& v) {
  _mergeSort(v, 0, v.size());
}

int main() {
  std::vector<int> v;
  const int N = 5000;
  for (int i = 0; i < N; ++i) {
    v.push_back(rand() % INT32_MAX);
  }

  auto begin = std::chrono::high_resolution_clock::now();
  mergeSort(v);
  auto end = std::chrono::high_resolution_clock::now();
  std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(end-begin).count() << "ms" << std::endl;

  std::vector<int> copy = v;
  std::sort(copy.begin(), copy.end());
  std::cout << (copy == v) << std::endl;
  return 0;
}