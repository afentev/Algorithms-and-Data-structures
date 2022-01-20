#include <iostream>


// vector copies its elements in advance. it stores a new buffer all the time
template <typename T>
class Vec {
 public:
  Vec(): ar(new T[2]), ar2copy(new T[4]), size(0), capacity(2) {}
  ~Vec() {
    delete[] ar;
    delete[] ar2copy;
  }

  void push_back(const T& el) {
    if (size == capacity) {
      delete[] ar;
      capacity *= 2;
      ar = new T[capacity];
      std::swap(ar, ar2copy);
    }

    ar[size] = el;

    if (2 * size >= capacity) {
      ar2copy[2 * size - capacity] = ar[2 * size - capacity];
      ar2copy[2 * size - capacity + 1] = ar[2 * size - capacity + 1];
    }
    ++size;
  }

  void pop_back() {
    --size;
  }

  T& operator[] (size_t index) {
    return ar[index];
  }

  const T& operator[] (size_t index) const {
    return ar[index];
  }

 private:
  T* ar;
  T* ar2copy;
  size_t size;
  size_t capacity;
};


int main() {
  Vec<int> v;
  for (int i = 0; i < 500; ++i) {
    v.push_back(i);
  }
  for (int i = 0; i < 500; ++i) {
    std::cout << v[i] << std::endl;
  }
  return 0;
}
