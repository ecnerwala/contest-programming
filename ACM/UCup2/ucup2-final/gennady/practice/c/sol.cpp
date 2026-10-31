/**
 *    author:  tourist
 *    created: 21.02.2025 17:07:09
**/
#include <bits/stdc++.h>

using namespace std;

#ifdef LOCAL
#include "debug.cpp"
#else
#define debug(...) 42
#endif

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  int tt;
  cin >> tt;
  while (tt--) {
    int n;
    cin >> n;
    const int M = int(1e9);
    n += 2;
    vector<int> a(n);
    a[0] = a[n - 1] = M;
    int64_t ans = 0;
    for (int i = 1; i < n - 1; i++) {
      cin >> a[i];
      ans += a[i];
    }
    vector<int> d(n - 1);
    int64_t sum = 0;
    for (int i = 0; i < n - 1; i++) {
      d[i] = a[i + 1] - a[i];
      sum += max(0, d[i]);
    }
    vector<pair<int, int>> e;
    vector<int> st;
    for (int i = 0; i < n - 1; i++) {
      if (d[i] < 0) {
        st.push_back(i);
      }
      if (d[i] > 0) {
        while (!st.empty() && d[i] > 0) {
          int j = st.back();
          int x = min(d[i], -d[j]);
          e.emplace_back(i - j, x);
          d[i] -= x;
          d[j] += x;
          if (d[j] == 0) {
            st.pop_back();
          }
        }
      }
    }
    sort(e.begin(), e.end());
    if (sum > M) {
      for (auto& [v, x] : e) {
        auto take = min<int64_t>(x, sum - M);
        ans += take * v;
        sum -= take;
      }
    }
    cout << ans << '\n';
  }
  return 0;
}
