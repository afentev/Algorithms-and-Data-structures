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
    int i = l - 1;
    for (int j = l; j <= r; ++j) {
      if (v[j] <= pivot) {
        std::swap(v[++i], v[j]);
      }
    }

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
  std::random_device rd_;
  std::mt19937 g(rd_());

  std::vector<int> a {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
  std::shuffle(a.begin(), a.end(), g);
  for (int i = 0; i < a.size(); ++i) {
    std::cout << quickSelect(a, i) << std::endl;
  }
  return 0;
}