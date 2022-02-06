#include <iostream>
#include <vector>
#include <random>
#include <chrono>
#include <algorithm>


template <typename T>  // uintN_t
void radixSort(T* v, const size_t N, const size_t groupLength) {
  const size_t groupSize = 1ull << groupLength;
  const size_t size = sizeof(T) << 3;
  const size_t groups = size / groupLength;
  const size_t rightShift = size - groupLength;

  T* digits = new T[groupSize];
  T* tmp = new T[N];
  memset(tmp, 0, N * sizeof(T));

  for (size_t i = 1; i <= groups; ++i) {
    const size_t leftShift = size - groupLength * i;

    memset(digits, 0, groupSize * sizeof(T));

    for (size_t j = 0; j < N; ++j) {
      ++digits[(v[j] << leftShift) >> rightShift];
    }
    size_t count = 0;
    for (size_t j = 0; j < groupSize; ++j) {
      size_t d = digits[j];
      digits[j] = count;
      count += d;
    }
    for (size_t j = 0; j < N; ++j) {
      tmp[digits[(v[j] << leftShift) >> rightShift]++] = v[j];
    }
    memcpy(v, tmp, N * sizeof(T));
  }

  delete[] digits;
  delete[] tmp;
}

int main() {
  const size_t N = 1e7;
  std::vector<uint32_t> array1(N, 0);
  std::vector<uint32_t> array2, array4, array8, array16, target;

  std::random_device rd;
  std::mt19937 g(rd());
  std::uniform_int_distribution<uint32_t> dist(0, UINT32_MAX);

  for (size_t i = 0; i < N; ++i) {
    array1[i] = dist(g);
  }
  array2 = array4 = array8 = array16 = target = array1;

  std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
  radixSort(array1.data(), N, 1);
  std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
  std::cout << "Radix sort (1 bit) = " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count()
            << "[ms]" << std::endl;

  begin = std::chrono::steady_clock::now();
  radixSort(array2.data(), N, 2);
  end = std::chrono::steady_clock::now();
  std::cout << "Radix sort (2 bit) = " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count()
            << "[ms]" << std::endl;


  begin = std::chrono::steady_clock::now();
  radixSort(array4.data(), N, 4);
  end = std::chrono::steady_clock::now();
  std::cout << "Radix sort (4 bit) = " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count()
            << "[ms]" << std::endl;

  begin = std::chrono::steady_clock::now();
  radixSort(array8.data(), N, 8);
  end = std::chrono::steady_clock::now();
  std::cout << "Radix sort (8 bit) = " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count()
            << "[ms]" << std::endl;

  begin = std::chrono::steady_clock::now();
  radixSort(array16.data(), N, 16);
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
