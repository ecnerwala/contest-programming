#include <bits/stdc++.h>

template <typename T> using min_pq = std::priority_queue<T, std::vector<T>, std::greater<T>>;

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N, M, D; cin >> N >> M >> D;

	vector<vector<pair<int, int64_t>>> adj(N);
	for (int e = 0; e < M; e++){
		int u, v; int64_t w;
		cin >> u >> v >> w;
		u--, v--;
		adj[u].push_back({v, w});
	}

	std::vector<int64_t> dist(N, -1);
	min_pq<pair<int64_t, int>> pq;
	pq.push({dist[0] = 0, 0});

	vector<int> verts; verts.reserve(N);

	while (!pq.empty()) {
		auto [d, cur] = pq.top(); pq.pop();
		if (d != dist[cur]) continue;
		verts.push_back(cur);
		for (auto [nxt, l] : adj[cur]) {
			if (dist[nxt] == -1 || d + l < dist[nxt]) {
				pq.push({dist[nxt] = d+l, nxt});
			}
		}
	}

	vector<int> prv(N, -1);
	vector<int> dp(N, 0);
	vector<int64_t> times(D); for (auto& v : times) cin >> v;
	sort(times.begin(), times.end());
	auto it = times.begin();

	prv[0] = -1;
	dp[0] = 1;
	for (int cur : verts) {
		while (it != times.end() && *it <= dist[cur]) ++it;
		assert(it == times.end() || *it > dist[cur]);
		if (cur == N-1) {
			if (it != times.end() || dp[cur] == 0) {
				cout << 0 << '\n';
			} else if (dp[cur] > 1) {
				cout << 1 << '\n';
			} else {
				vector<int> path; path.reserve(N);
				for (int v = cur; v != -1; v = prv[v]) {
					path.push_back(v);
				}
				reverse(path.begin(), path.end());
				cout << path.size() << '\n';
				for (int v : path) cout << v+1 << '\n';
			}
			exit(0);
		}
		if (dp[cur] == 0) continue;
		for (auto [nxt, l] : adj[cur]) {
			if (dist[nxt] != dist[cur] + l) continue;
			if (it != times.end() && *it < dist[nxt]) continue;
			prv[nxt] = cur;
			dp[nxt] += dp[cur];
			dp[nxt] = min(dp[nxt], 2);
		}
	}
	cout << 0 << '\n';
	exit(0);

	return 0;
}
