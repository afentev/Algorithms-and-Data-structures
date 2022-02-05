#include <iostream>
#include <vector>
#include <random>
#include <chrono>
#include <algorithm>


template <typename T>  // uintN_t
void radixSort(std::vector<T>& v, const size_t groupLength) {
  const size_t groupSize = 1ull << groupLength;
  const size_t size = sizeof(v.front()) << 3;
  const size_t groups = size / groupLength;
  const size_t rightShift = size - groupLength;

  std::vector<T> digits(groupSize, 0);
  std::vector<T> tmp(v.size(), 0);
  for (size_t i = 1; i <= groups; ++i) {
    const size_t leftShift = size - groupLength * i;

    digits.assign(groupSize, 0);

    for (size_t j = 0; j < v.size(); ++j) {
      size_t digit = (v[j] << leftShift) >> rightShift;
      ++digits[digit];
    }
    size_t count = 0;
    for (size_t j = 0; j < groupSize; ++j) {
      size_t d = digits[j];
      digits[j] = count;
      count += d;
    }
    for (size_t j = 0; j < v.size(); ++j) {
      size_t d = (v[j] << leftShift) >> rightShift;
      tmp[digits[d]] = v[j];
      ++digits[d];
    }
    v = tmp;
  }

}

int main() {
  const size_t N = 1e7;
  std::vector<uint32_t> array1(N, 0);
  std::vector<uint32_t> array2, array4, array8, array16, target;

  std::random_device rd;
  std::mt19937 g(rd());
  std::uniform_int_distribution<uint32_t> dist(0, INT32_MAX);

  for (size_t i = 0; i < N; ++i) {
    array1[i] = dist(g);
  }
  std::shuffle(array1.begin(), array1.end(), g);
  array2 = array4 = array8 = array16 = target = array1;

  std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
  radixSort(array1, 1);
  std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
  std::cout << "Radix sort (1 bit) = " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count()
            << "[ms]" << std::endl;

  begin = std::chrono::steady_clock::now();
  radixSort(array2, 2);
  end = std::chrono::steady_clock::now();
  std::cout << "Radix sort (2 bit) = " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count()
            << "[ms]" << std::endl;


  begin = std::chrono::steady_clock::now();
  radixSort(array4, 4);
  end = std::chrono::steady_clock::now();
  std::cout << "Radix sort (4 bit) = " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count()
            << "[ms]" << std::endl;

  begin = std::chrono::steady_clock::now();
  radixSort(array8, 8);
  end = std::chrono::steady_clock::now();
  std::cout << "Radix sort (8 bit) = " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count()
            << "[ms]" << std::endl;

  begin = std::chrono::steady_clock::now();
  radixSort(array16, 16);
  end = std::chrono::steady_clock::now();
  std::cout << "Radix sort (16 bit) = " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count()
            << "[ms]" << std::endl;

  begin = std::chrono::steady_clock::now();
  std::sort(target.begin(), target.end());
  end = std::chrono::steady_clock::now();
  std::cout << "std::sort = " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count()
            << "[ms]" << std::endl;

  assert(array1 == target);
  assert(array2 == target);
  assert(array4 == target);
  assert(array8 == target);
  assert(array16 == target);

  return 0;
}
