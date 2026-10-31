#include<bits/stdc++.h>
using namespace std;

using ll = long long;
const ll INF = 1e18;

const int S = 1 << 17;
const int MAXN = 1.1e5;
const int MAXM = 1.1e5;
const int MAXV = 4 * S + MAXM;

int N, M, V;

vector<pair<int, int>> adj[MAXV];
ll dist[MAXV];

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	cin >> N >> M;
	V = 4 * S + M;
	for (int i = 1; i < S; i++) {
		adj[2*i].emplace_back(i, 0);
		adj[2*i+1].emplace_back(i, 0);
		adj[2*S+i].emplace_back(2*S+2*i, 0);
		adj[2*S+i].emplace_back(2*S+2*i+1, 0);
	}
	for (int i = 1; i <= N; i++) {
		adj[S+i].emplace_back(3*S+i, 0);
		adj[3*S+i].emplace_back(S+i, 0);
	}
	for (int v = 4 * S; v < V; v++) {
		int l, r, c; cin >> l >> r >> c;
		cerr << l << ' ' << r << ' ' << c << '\n';
		r++;
		for (int a = l + S, b = r + S; a < b; a /= 2, b /= 2) {
			if (a & 1) {
				adj[a].emplace_back(v, 0);
				adj[v].emplace_back(2*S+a, c);
				a++;
			}
			if (b & 1) {
				--b;
				adj[b].emplace_back(v, 0);
				adj[v].emplace_back(2*S+b, c);
			}
		}
	}
	for (int i = 0; i < V; i++) {
		dist[i] = INF;
	}
	priority_queue<pair<ll, int>, vector<pair<ll, int>>, greater<pair<ll, int>>> pq;
	dist[S+1] = 0;
	pq.emplace(dist[S+1], S+1);
	while (!pq.empty()) {
		ll d = pq.top().first;
		int cur = pq.top().second;
		pq.pop();
		if (d != dist[cur]) continue;
		for (auto it : adj[cur]) {
			int nxt = it.first;
			ll nd = d + it.second;
			if (nd < dist[nxt]) {
				dist[nxt] = nd;
				pq.emplace(dist[nxt], nxt);
			}
		}
	}

	if (dist[S+N] == INF) cout << -1 << '\n';
	else cout << dist[S+N] << '\n';

	return 0;
}
