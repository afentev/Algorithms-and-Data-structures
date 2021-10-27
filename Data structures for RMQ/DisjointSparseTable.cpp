#include <bits/stdc++.h>
#define int int64_t

using namespace std;

int mul(int a, int b) {
  return a * b;
}

int sum(int a, int b) {
  return a + b;
}

int min(int a, int b) {
  return (a < b? a: b);
}

struct DST {
 public:
  explicit DST(vector<int>& a) {
    int pw = 1, height = 1;
    while (a.size() > pw) {
      pw *= 2;
      height++;
    }
    a.resize(pw, 0);
    DST.resize(height - 1, vector<int> (pw, 0));
    DST[0] = a;
    for (int i = 1; i < height - 1; ++i) {
      int len = 1 << i;
      for (int center = len; center < pw; center += 2 * len) {
        DST[i][center] = a[center];
        for (int j = center + 1; j < center + len; ++j) {
          DST[i][j] = sum(DST[i][j - 1], a[j]);
        }
        DST[i][center - 1] = a[center - 1];
        for (int j = center - 2; j >= center - len; --j) {
          DST[i][j] = sum(DST[i][j + 1], a[j]);
        }
      }
    }
  }

  int get(int l, int r) {
    if (l == r) {
      return DST[0][l];
    }
    int dif = 63 - __builtin_clzll(r ^ l);
    return sum(DST[dif][r], DST[dif][l]);
  }

 private:
  vector<vector<int>> DST;
};

// solution for https://codeforces.com/gym/100093/problem/D
int32_t main() {
//  freopen("sum2.in","r",stdin);
//  freopen("sum2.out","w",stdout);

  cin.tie(nullptr);
  std::ios_base::sync_with_stdio(false);

  int n;
  cin >> n;
  vector<int> a(n, 0);
  for (int i = 0; i < n; ++i) {
    cin >> a[i];
  }

  DST x(a);

  int q;
  cin >> q;
  for (int i = 0; i < q; ++i) {
    int l, r;
    cin >> l >> r;
    cout << x.get(l - 1, r - 1) << endl;
  }
  return 0;
}
