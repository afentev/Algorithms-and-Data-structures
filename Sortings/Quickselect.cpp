//
// Created by afentev on 10/30/21.
//
#include <vector>
#include <random>
#include <iostream>

std::random_device rd;
std::mt19937 eng(rd());

int medianOf3(std::vector<int>& v, int x1, int x2, int x3) {
  if ((v[x1] > v[x2]) ^ (v[x1] > v[x3])) {
    return x1;
  }
  if ((v[x2] < v[x1]) ^ (v[x2] < v[x3])) {
    return x2;
  }
  return x3;
}

int quickSelect(std::vector<int> v, int k) {
  int l = 0;
  int r = v.size() - 1;

  while (true) {
    std::uniform_int_distribution<int> distr(l, r);
    int pivotInd = medianOf3(v, distr(eng), distr(eng), distr(eng));  // median of 3 random
    int pivot = v[pivotInd];
    // partition:
    std::swap(v[pivotInd], v[r]);
    int i = l;
    for (int j = l; j < r; ++j) {
      if (v[j] < pivot) {
        std::swap(v[i++], v[j]);
      }
    }
    std::swap(v[r], v[i]);

    if (i < k) {
      l = i + 1;
    } else if (i > k) {
      r = i - 1;
    } else {
      return v[k];
    }
  }
}

int main() {
  int n = 1e6;
  std::random_device rd_;
  std::uniform_int_distribution<int> dist(0, INT32_MAX);
  std::uniform_int_distribution<int> ind(0, n - 1);

  std::vector<int> a;
  for (int i = 0; i < n; ++i) {
    a.push_back(dist(eng));
  }
  auto begin = std::chrono::high_resolution_clock::now();

  quickSelect(a, ind(eng));

  auto end = std::chrono::high_resolution_clock::now();
  std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(end-begin).count() << "ms" << std::endl;
  return 0;
}