#include<bits/stdc++.h>
using namespace std;

using ll = long long;
const int MOD = int(1e9)+7;

const int MAXN = 6.1e5;
const int MAXM = 5.1e5;
int N, M;

ll K;

vector<pair<int, ll>> adj[MAXN];
bool vis[MAXN];
ll dist[MAXN];

vector<int> cc;
void dfs(int cur, ll d) {
	if (vis[cur]) {
		if (dist[cur] != d) {
			cout << 0 << '\n';
			exit(0);
		}
		return;
	}
	dist[cur] = d;
	vis[cur] = true;
	cc.push_back(cur);
	for (auto e : adj[cur]) {
		dfs(e.first, d + e.second);
	}
}

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	cin >> N >> M >> K;
	for (int e = 0; e < M; e++) {
		int a, b; ll c; cin >> a >> b >> c;
		a--, b--;
		adj[a].emplace_back(b+N, -c);
		adj[b].emplace_back(a+N, -c);
		adj[a+N].emplace_back(b, +c);
		adj[b+N].emplace_back(a, +c);
	}

	ll ans = 1;

	for (int i = 0; i < N; i++) {
		if (vis[i]) continue;
		cc = {};
		dfs(i, 0);
		assert(vis[i]);
		if (vis[i+N]) {
			// -x_i = x_i + dist[i+N]
			ll a = -dist[i+N];
			if (a & 1) {
				cout << 0 << '\n';
				exit(0);
			}
			a /= 2;
			for (int v : cc) {
				assert(vis[(v+N) % (2*N)]);
				ll x = a + dist[v];
				if (v >= N) {
					x = -x;
				}
				if (1 <= x && x <= K) {
					// it's good
				} else {
					cout << 0 << '\n';
					exit(0);
				}
			}
		} else {
			// mark the other side
			for (int v : cc) {
				assert(!vis[(v+N) % (2*N)]);
				vis[(v + N) % (2 * N)] = true;
			}

			ll lo = 1, hi = K;
			for (int v : cc) {
				ll curLo, curHi;
				if (v >= N) {
					curLo = -K, curHi = -1;
				} else {
					curLo = 1, curHi = K;
				}
				//cerr << v << ' ' << dist[v] << '\n';
				// curLo <= a + dist[v] <= curHi
				curLo -= dist[v];
				curHi -= dist[v];
				lo = max(lo, curLo);
				hi = min(hi, curHi);
			}
			//cerr << lo << '-' << hi << '\n';
			if (lo > hi) {
				cout << 0 << '\n';
				exit(0);
			}
			ans *= (hi - lo + 1) % MOD;
			ans %= MOD;
		}
	}
	cout << ans << '\n';

	return 0;
}
