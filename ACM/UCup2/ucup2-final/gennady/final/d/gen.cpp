/**
 *    author:  tourist
 *    created: 22.02.2025 15:24:41
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
  // int h = 1;
  // int w = 8;
  // int r = 1;
  // int c = 7;
  // cout << h * w << '\n';
  // for (int x = 1; x <= h * w; x++) {
  //   cout << h << " " << w << " " << r << " " << c << " " << x << '\n';
  // }
  mt19937_64 rng((unsigned int) chrono::steady_clock::now().time_since_epoch().count());
  int tt = 10000;
  cout << tt << '\n';
  while (tt--) {
    int h = rng() % 5 + 1;
    int w = rng() % 5 + 1;
    int r = rng() % h + 1;
    int c = rng() % w + 1;
    int x = rng() % (h * w) + 1;
    cout << h << " " << w << " " << r << " " << c << " " << x << '\n';
  }
  return 0;
}
