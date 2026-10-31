/**
 *    author:  tourist
 *    created: 22.02.2025 12:09:53
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
  mt19937_64 rng((unsigned int) chrono::steady_clock::now().time_since_epoch().count());
  int t = 1;
  cout << t << '\n';
  while (t--) {
    int n = int(5e5);
    int m = int(5e5);
    int s = int(5e5);
    int x = int(1e9);
    cout << n << " " << m << " " << s << '\n';
    for (int i = 0; i < s; i++) {
      cout << int(rng() % (2 * x)) - x << " \n"[i == s - 1];
    }
  }
  return 0;
}
