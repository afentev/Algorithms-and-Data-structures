#include <iostream>
#include <vector>
#include <chrono>

template <typename T>
class HTA {
 public:
  HTA(): k(1), sz(0), pw(1ull << k), roots(new T*[pw]) {}

  ~HTA() {
    size_t freed = 0;
    for (int i = 0; freed < sz; ++i, freed += pw) {
      delete[] roots[i];
    }
    delete[] roots;
  }

  void push_back(const T& v) {
    if (sz == (pw << k)) {
      ++k;
      pw <<= 1;
      reallocate(k - 1);
    }

    if ((sz & (pw - 1)) == 0) {
      roots[sz >> k] = new T[pw];
    }

    new (roots[sz >> k] + (sz & (pw - 1))) T(v);
    sz++;
  }
  
  void pop_back() {
    sz--;
  }

  size_t size() const {
    return pw;
  }

  T& operator[] (size_t index) {
    return roots[index >> k][index & (pw - 1)];
  }

  const T& operator[] (size_t index) const {
    return roots[index >> k][index & (pw - 1)];
  }

 private:
  void reallocate(size_t oldK) {
    size_t old = 1ull << oldK;
    T** tmp = new T*[pw];
    size_t used = 0;
    for (size_t i = 0; used < sz; ++i, used += pw) {
      tmp[i] = new T[pw];
    }
    for (size_t i = 0; i < sz; ++i) {
      new(tmp[i >> k] + (i & (pw - 1))) T(roots[i >> oldK][i & (old - 1)]);
    }
    for (size_t i = 0; i < old; ++i) {
      delete[] roots[i];
    }
    delete[] roots;
    roots = tmp;
  }

 private:
  size_t k;
  size_t sz;
  size_t pw;
  T** roots;
};

// example struct without std::move
struct TestClass {
  int64_t a, b, c, d, e, f, g, h, i, j, k, l;
};

int main() {
  HTA<TestClass> hta;
  std::vector<TestClass> vec;
  const int N = 1e7;
  auto begin = std::chrono::high_resolution_clock::now();
  for (int i = 0; i < N; ++i) {
    hta.push_back(TestClass());
  }
  auto end = std::chrono::high_resolution_clock::now();
  std::cout << "HTA: " << std::chrono::duration_cast<std::chrono::milliseconds>(end-begin).count() << "ms" << std::endl;

  begin = std::chrono::high_resolution_clock::now();
  for (int i = 0; i < N; ++i) {
    vec.push_back(TestClass());
  }
  end = std::chrono::high_resolution_clock::now();
  std::cout << "Vector: " << std::chrono::duration_cast<std::chrono::milliseconds>(end-begin).count() << "ms" << std::endl;
  return 0;
}