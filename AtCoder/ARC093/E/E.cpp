#include<bits/stdc++.h>
using namespace std;

using ll = long long;

const int MAXN = 1010;
int N;
ll dist[MAXN][MAXN];
int ccNum[MAXN];
vector<int> ccs[MAXN];

const int MAXM = 2020;
int M;
pair<ll, pair<int, int>> edges[MAXM];
ll X;

const int MOD = int(1e9)+7;

int main() {
  cin >> N >> M >> X;
  for (int e = 0; e < M; e++) {
    int u, v, w; cin >> u >> v >> w; u--, v--;
    edges[e] = {w,{u,v}};
  }
  sort(edges,edges+M);
  for (int i = 0; i < N; i++) { ccs[ccNum[i] = i].push_back(i); }
  for (int e = 0; e < M; e++) {
    int u = ccNum[edges[e].second.first], v = ccNum[edges[e].second.second];
    if (u==v) continue;
    X -= edges[e].first;
    for (int a : ccs[u]) for (int b : ccs[v]) dist[a][b] = dist[b][a] = edges[e].first;
    for (int b : ccs[v]) { ccs[ccNum[b] = u].push_back(b); }
  }
  ll c0 = 0, c1 = 0;
  for (int e = 0; e < M; e++) {
    ll v = edges[e].first - dist[edges[e].second.first][edges[e].second.second];
    c0 += (v < X); c1 += (v <= X);
  }
  c0 = max(c0-1, 0ll), c1 = max(c1-1, 0ll);
  int v0 = 1, v1 = 1;
  for (int i = 0; i < M-c0;i++) { v0 <<= 1; if (v0 >= MOD) v0 -= MOD; }
  for (int i = 0; i < M-c1;i++) { v1 <<= 1; if (v1 >= MOD) v1 -= MOD; }
  int ans = v0 - v1; if (ans < 0) ans += MOD;
  cout << ans << '\n';
}
