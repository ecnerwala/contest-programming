#include<bits/stdc++.h>
using namespace std;

using ll = long long;

const int MAXN = 2.1e5;
int N;
vector<pair<int, ll>> adj[MAXN];
ll dist[MAXN];
bool vis[MAXN];

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);

	int M;
	cin >> N >> M;
	for (int e = 0; e < M; e++) {
		int a, b; ll c; cin >> a >> b >> c;
		adj[a].emplace_back(b, c);
		adj[b].emplace_back(a, c);
	}

	memset(dist, -1, sizeof(dist));

	priority_queue<pair<ll, int>> pq;
	pq.emplace(dist[1] = 0, 1);
	while (!pq.empty()) {
		ll d = pq.top().first; int cur = pq.top().second; pq.pop();
		if (vis[cur]) continue;
		vis[cur] = true;

		d = dist[cur]; // do this?

		//if (cur == N) { cout << d << '\n'; exit(0); }

		for (auto it : adj[cur]) {
			int nxt = it.first;
			ll nd = d + it.second;
			if (dist[nxt] == -1 || nd < dist[nxt]) {
				pq.emplace(dist[nxt] = nd, nxt);
			}
		}
	}

	cout << dist[N] << '\n';

	return 0;
}
