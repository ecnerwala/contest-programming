/**
 *    author:  tourist
 *    created: 22.06.2024 11:05:17
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
  while (tt--) {
    int n;
    cin >> n;
    vector<int> d(n);
    for (int i = 0; i < n; i++) {
      cin >> d[i];
    }
    sort(d.rbegin(), d.rend());
    bool ok = false;
    if (n >= 5 && d[0] > 2) {
      if (d[3] >= 2) {
        ok = true;
      }
      if (d[2] >= 2 && d[0] != d[2]) {
        ok = true;
      }
    }
    cout << (ok ? "Yes" : "No") << '\n';
  }
  return 0;
}
