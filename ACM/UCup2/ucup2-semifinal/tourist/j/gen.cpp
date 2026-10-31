/**
 *    author:  tourist
 *    created: 22.06.2024 13:02:16
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
  int n = 500000;
  mt19937_64 rng((unsigned int) chrono::steady_clock::now().time_since_epoch().count());
  cout << n << '\n';
  vector<int> p(n);
  iota(p.begin(), p.end(), 1);
  shuffle(p.begin(), p.end(), rng);
  for (int i = 0; i < n; i++) cout << p[i] << " \n"[i == n - 1];
  return 0;
}
