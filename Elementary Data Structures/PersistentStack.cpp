//
// Created by afentev on 09/14/21.
//

#include <iostream>
#include <vector>
#include <tuple>
#include <utility>

template<typename T>
class PersistentStack {
 public:
  PersistentStack() {
    data.emplace_back(-1, -1);  // genesis
  }

  void push(size_t pos, T element) {
    data.emplace_back(element, pos);
  }

  T pop(size_t pos) {
    data.emplace_back(data[data[pos].second].first,
                      data[data[pos].second].second);
    return data[pos].first;
  }

  void reserve(size_t size) {
    data.reserve(size);
  }

 private:
  std::vector<std::pair<T, size_t>> data;
};

// solution for https://www.e-olymp.com/ru/problems/4313
int main() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(nullptr);

  PersistentStack<int16_t> s;
  s.reserve(200001);

  int t;
  std::cin >> t;
  while (t--) {
    int a, b;
    std::cin >> a >> b;
    if (b == 0) {
      std::cout << s.pop(a) << std::endl;
    } else {
      s.push(a, b);
    }
  }
  return 0;
}