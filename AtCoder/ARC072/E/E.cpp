#include<bits/stdc++.h>
using namespace std;

using ll = long long;

const int MAXN = 5.1e5;
int N;
ll D0;
ll D[MAXN];
ll reach[MAXN];
bool ans[MAXN];

int main() {
  ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
  cin >> N >> D0;
  for (int i = 0; i < N; i++) cin >> D[i];
  reach[N] = 0;
  for (int i = N-1; i >= 0; i--) reach[i] = reach[i+1] < D[i]/2 ? reach[i+1] : reach[i+1] + D[i];
  ll curD = D0;
  for (int i = 0; i < N; i++) {
    ans[i] = curD <= reach[i+1];
    curD = min(curD, abs(curD - D[i]));
  }
  int Q; cin >> Q; while (Q--) { int v; cin >> v; cout << (ans[v-1] ? "NO" : "YES") << '\n'; }
}
