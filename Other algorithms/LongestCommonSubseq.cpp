#include <iostream>
#include <vector>


// solution for https://informatics.msk.ru/mod/statements/view.php?id=766&chapterid=204#1
int main() {
  size_t n, m;
  std::vector<int> s, t;

  std::cin >> n;
  for (size_t i = 0; i < n; ++i) {
    int x;
    std::cin >> x;
    s.push_back(x);
  }

  std::cin >> m;
  for (size_t i = 0; i < m; ++i) {
    int x;
    std::cin >> x;
    t.push_back(x);
  }

  std::vector<std::vector<int>> dp(n + 1, std::vector<int>(m + 1, 0));
  for (size_t i = 1; i <= n; ++i) {
    for (size_t j = 1; j <= m; ++j) {
      if (s[i - 1] == t[j - 1]) {
        dp[i][j] = 1 + dp[i - 1][j - 1];
      } else {
        dp[i][j] = std::max(dp[i - 1][j], dp[i][j - 1]);
      }
    }
  }

   std::cout << dp[n][m] << std::endl;
  return 0;
}
