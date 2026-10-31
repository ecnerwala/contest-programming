#include<bits/stdc++.h>
using namespace std;

using ll = long long;

const ll INF = 1e18;

const int MAXN = 1.1e5;
const int MAXM = 1.1e5;
int N, M;
vector<pair<int, ll>> adj[MAXN];

ll dist[30][MAXN];

ll dp[1 << 13][13];

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	cin >> N >> M;
	for (int i = 0; i < M; i++) {
		int a, b, c; cin >> a >> b >> c;
		a--, b--;
		adj[a].emplace_back(b, c);
		adj[b].emplace_back(a, c);
	}

	for (int st = 0; st < 13; st++) {
		for (int i = 0; i < N; i++) {
			dist[st][i] = INF;
		}

		priority_queue<pair<ll, int>, vector<pair<ll, int>>, greater<pair<ll, int>>> pq;
		pq.emplace(dist[st][st] = 0, st);
		while (!pq.empty()) {
			int cur = pq.top().second;
			ll d = pq.top().first;
			pq.pop();
			if (d != dist[st][cur]) continue;
			for (auto it : adj[cur]) {
				int nxt = it.first;
				ll nd = d + it.second;
				if (nd < dist[st][nxt]) {
					pq.emplace(dist[st][nxt] = nd, nxt);
				}
			}
		}
	}

	for (int m = 0; m < (1 << 13); m++) {
		for (int i = 0; i < 13; i++) {
			dp[m][i] = INF;
		}
	}

	dp[0][0] = 0;

	for (int m = 0; m < (1 << 13); m++) {
		for (int i = 0; i < 13; i++) {
			if (dp[m][i] == INF) continue;
			for (int j = 0; j < 13; j++) {
				if (m & (1 << j)) {
					continue;
				}
				dp[m | (1 << j)][j] = min(dp[m | (1 << j)][j], dp[m][i] + dist[i][j]);
			}
		}
	}

	cout << dp[(1 << 13) - 1][0] << '\n';

	return 0;
}
