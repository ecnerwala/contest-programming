#include<bits/stdc++.h>
using namespace std;

using ll = long long;

int signum(ll a) { return (a > 0) - (a < 0); }

int main() {
  int N; cin >> N;
  vector<ll> A(N);
  for (int i = 0; i < N; i++) {
    cin >> A[i];
  }
  ll ans = 1e18;
  for (int z = -1; z <= 1; z += 2) {
    ll sum = 0;
    ll cost = 0;
    for (int i = 0; i < N; i++) {
      sum += A[i];
      int want = (i&1) ? -z : z;
      if (signum(sum) != want) { cost += abs(sum-want); sum = want; }
    }
    ans = min(ans, cost);
  }
  cout << ans << '\n';
}
