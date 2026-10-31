/**
 *    author:  tourist
 *    created: 22.06.2024 11:31:36
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
  int tt;
  cin >> tt;
  auto solve = [&]() -> void {
    int n, k;
    cin >> n >> k;
    vector<int> c(k);
    for (int i = 0; i < k; i++) {
      cin >> c[i];
    }
    for (int i = 0; i < k; i++) {
      if (c[i] >= 2) {
        cout << n - 1 << '\n';
        for (int j = 1; j < n; j++) {
          cout << 1 << " " << j + 1 << " " << i + 1 << '\n';
        }
        return;
      }
    }
    vector<int> ones;
    for (int i = 0; i < k; i++) {
      if (c[i] >= 1) {
        ones.push_back(i);
      }
    }
    vector<array<int, 3>> edges;
    int done = 1;
    while (done < n) {
      if (!ones.empty()) {
        edges.push_back({done - 1, done, ones.back()});
        ones.pop_back();
        done += 1;
        continue;
      }
      int take = min(n - done, k - 1);
      for (int i = 0; i < take; i++) {
        edges.push_back({done - 1 + i, done - 1 + i + 1, i});
      }
      edges.push_back({done - 1, done - 1 + take, take});
      done += take;
    }
    cout << edges.size() << '\n';
    for (auto [a, b, c] : edges) {
      cout << a + 1 << " " << b + 1 << " " << c + 1 << '\n';
    }
  };
  while (tt--) {
    solve();
  }
  return 0;
}
