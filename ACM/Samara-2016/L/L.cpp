#include<bits/stdc++.h>
using namespace std;
const int INF = 1e9;
const int MAXN = 2.1e5;
int N;
vector<int> adj[MAXN], radj[MAXN];
int dist[MAXN];
int res[MAXN];
void bfs(int st) {
  for (int i = 0; i <= N; i++) dist[i] = INF;
  vector<int> q;
  dist[st] = 0;
  q.push_back(st);
  for (int cur : q) {
    for (int nxt : adj[cur]) {
      if (dist[nxt] == INF) dist[nxt] = dist[cur] + 1, q.push_back(nxt);
    }
  }
  for (int i = 0; i <= N; i++) res[i] += dist[i];
}
int main() {
  ios_base::sync_with_stdio(0);
  cin >> N;
  int M, A, B; cin >> M >> A >> B;
  for (int i = 0; i < M; i++) {
    int x, y; cin >> x >> y; adj[x].push_back(y); radj[y].push_back(x);
  }
  bfs(0);
  for (int i = 0; i <= N; i++) swap(adj[i], radj[i]);
  bfs(A);
  bfs(B);
  int v = res[0];
  for (int i = 1; i <= N; i++) v = min(v, res[i]);
  cout << v << '\n';
  return 0;
}
