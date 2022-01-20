#include <iostream>
#include <vector>
#include <chrono>

template <typename T>
void _quickSort(std::vector<T>& v, size_t l, size_t r) {
  if (l >= r) {
    return;
  }
  size_t pivotInd = l + rand() % (r - l + 1);
  T pivot = v[pivotInd];

  int i = l;
  int j = r;
  while (i <= j) {
    while (v[i] < pivot) {
      ++i;
    }

    while (v[j] > pivot) {
      --j;
    }

    if (i >= j) {
      break;
    }

    std::swap(v[i], v[j]);
    ++i, --j;
  }

  _quickSort(v, l, j);
  _quickSort(v, j + 1, r);
}

template <typename T>
void quickSort(std::vector<T>& v) {
  _quickSort(v, 0, v.size() - 1);
}

int main() {
  std::vector<int> v;
  const int N = 1000;
  for (int i = 0; i < N; ++i) {
    v.push_back(rand() % INT32_MAX);
  }

  auto begin = std::chrono::high_resolution_clock::now();
  quickSort(v);
  auto end = std::chrono::high_resolution_clock::now();
  std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(end-begin).count() << "ms" << std::endl;

  std::vector<int> copy = v;
  std::sort(copy.begin(), copy.end());
  std::cout << (copy == v) << std::endl;
  return 0;
}
