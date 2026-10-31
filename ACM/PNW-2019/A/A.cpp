#include<bits/stdc++.h>
using namespace std;

using ll = long long;

const int MAXN = 1.1e5;
int N;
vector<pair<int, ll>> adj[MAXN];
ll T[MAXN];

ll sz[MAXN];
ll wsz[MAXN];

ll tDist[MAXN];
ll twDist[MAXN];

void dfs(int cur, int prv) {
	sz[cur] = 1;
	wsz[cur] = T[cur];
	tDist[cur] = 0;
	twDist[cur] = 0;
	for (auto it : adj[cur]) {
		int nxt = it.first;
		ll w = it.second;
		if (nxt == prv) continue;

		dfs(nxt, cur);

		sz[cur] += sz[nxt];
		wsz[cur] += wsz[nxt];

		tDist[cur] += tDist[nxt] + sz[nxt] * w;
		twDist[cur] += twDist[nxt] + wsz[nxt] * w;
	}

	//cerr << cur << ' ' << prv << ' ' << sz[cur] << ' ' << wsz[cur] << ' ' << tDist[cur] << ' ' << twDist[cur] << '\n';
}

ll ans[MAXN];

void dfs2(int cur, int prv, ll totDist, ll totWDist) {
	ans[cur] = totWDist + totDist * T[cur];

	for (auto it : adj[cur]) {
		int nxt = it.first;
		ll w = it.second;
		if (nxt == prv) continue;

		dfs2(nxt, cur, totDist + (sz[0] - sz[nxt] * 2) * w, totWDist + (wsz[0] - wsz[nxt] * 2) * w);
	}
}

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	cin >> N;
	for (int i = 0; i < N; i++) {
		cin >> T[i];
	}
	for (int i = 0; i < N-1; i++) {
		int u, v, w; cin >> u >> v >> w; u--, v--;
		adj[u].push_back(pair<int, ll>(v, w));
		adj[v].push_back(pair<int, ll>(u, w));
	}

	dfs(0, -1);
	dfs2(0, -1, tDist[0], twDist[0]);

	for (int i = 0; i < N; i++) {
		cout << ans[i] << '\n';
	}

	return 0;
}
