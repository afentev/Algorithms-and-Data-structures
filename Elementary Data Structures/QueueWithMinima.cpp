#include <vector>
#include <stack>
#include <utility>
#include <iostream>

class queue {
 public:
  void push(int el) {
    if (s1.empty()) {
      s1.emplace_back(el, el);
    } else {
      s1.emplace_back(el, std::min(el, s1.back().second));
    }
  }

  int pop() {
    if (s2.empty()) {
      while (!s1.empty()) {
        int el = s1.back().first; s1.pop_back();
        if (s2.empty()) {
          s2.emplace_back(el, el);
        } else {
          s2.emplace_back(el, std::min(el, s2.back().second));
        }
      }
    }
    int el = minima();
    s2.pop_back();
    return el;
  }

  int minima() {
    if (s1.empty()) {
      return s2.back().second;
    } else if (s2.empty()) {
      return s1.back().second;
    } else {
      return std::min(s2.back().second, s1.back().second);
    }
  }

 private:
  std::vector<std::pair<int, int>> s1, s2;
};

// moving minimum in vector size n in a window length k
int main() {
  int n, k;
  std::cin >> n >> k;
  std::vector<int> a(n, 0);

  queue q;
  int out = 0;
  for (int i = 0; i < n; ++i) {
    std::cin >> a[i];
    q.push(a[i]);
    if (i + 1 >= k) {
      std::cout << q.pop() << std::endl;
      out++;
    }
  }
  while (out++ < n - k + 1) {
    std::cout << q.pop() << std::endl;
  }
  return 0;
}