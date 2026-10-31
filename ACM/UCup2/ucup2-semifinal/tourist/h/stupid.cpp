/**
 *    author:  tourist
 *    created: 22.06.2024 14:06:20
**/
#include <bits/stdc++.h>

using namespace std;

#ifdef LOCAL
#include "../debug.cpp"
#else
#define debug(...) 42
#endif

int main() {
  ios::sync_with_stdio(false);
  cin.tie(0);
  int n;
  cin >> n;
  const int M = 802;
  vector<vector<int>> a(n, vector<int>(M, 0));
  for (int i = 0; i < n; i++) {
    int foo;
    cin >> foo;
    for (int j = 0; j < foo; j++) {
      int x;
      cin >> x;
      a[i][x] = 1;
    }
  }
  vector<int> b(n);
  int ans = 0;
  auto Dfs = [&](auto&& self, int v) -> void {
    if (v == n) {
      int x = 0;
      for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
          x = max(x, (b[j] - j) - (b[i] - i));
        }
      }
      x = max(0, x - (n - 1));
      ans += x;
      return;
    }
    for (int x = (v == 0 ? 0 : b[v - 1]); x < M; x++) {
      if (a[v][x]) {
        b[v] = x;
        self(self, v + 1);
      }
    }
  };
  Dfs(Dfs, 0);
  cout << ans << '\n';
  return 0;
}
