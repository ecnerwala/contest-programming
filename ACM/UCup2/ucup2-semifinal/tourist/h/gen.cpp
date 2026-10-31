/**
 *    author:  tourist
 *    created: 22.06.2024 14:19:33
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
  mt19937_64 rng((unsigned int) chrono::steady_clock::now().time_since_epoch().count());
  int n = 5;
  cout << n << '\n';
  while (n--) {
    vector<int> a;
    for (int x = 1; x <= 12; x++) {
      if (rng() % 2) {
        a.push_back(x);
      }
    }
    cout << a.size();
    for (int x : a) {
      cout << " " << x;
    }
    cout << '\n';
  }
  return 0;
}
