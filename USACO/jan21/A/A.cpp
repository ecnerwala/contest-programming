#include <bits/stdc++.h>

int minv(int a, int m) {
	if (a == 1) return 1;
	return m - int(int64_t(minv(m % a, a)) * m / a);
}

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	const int MOD = int(1e9)+7;
	auto mul = [&](int a, int b) {
		return int(int64_t(a) * int64_t(b) % MOD);
	};
	auto sub = [&](int a, int b) {
		a -= b;
		return (a < 0) ? a + MOD : a;
	};

	int K; cin >> K;

	const int MAXD = 2.1e5;
	vector<vector<int>> evts(MAXD);

	int tot_val = 1;
	for (int k = 0; k < K; k++) {
		int N, M; cin >> N >> M;
		vector<vector<int>> adj(N);
		for (int e = 0; e < M; e++) {
			int u, v; cin >> u >> v; u--, v--;
			adj[u].push_back(v);
			if (u != v) adj[v].push_back(u);
		}
		vector<int> dist(2*N, MAXD);
		{
			vector<int> q; q.reserve(2*N);
			dist[0] = 0; q.push_back(0);
			for (int z = 0; z < int(q.size()); z++) {
				int cur = q[z];
				int nxtp = (cur & 1) ^ 1;
				for (int nxtv : adj[cur>>1]) {
					int nxt = (nxtv << 1) | nxtp;
					if (dist[nxt] == MAXD) {
						dist[nxt] = dist[cur] + 1;
						q.push_back(nxt);
					}
				}
			}
		}

		tot_val = mul(tot_val, N);

		for (int i = 0; i < N; i++) {
			array<int, 2> d{dist[2*i], dist[2*i+1]};
			assert(min(d[0], d[1]) < MAXD);

			if (d[0] < MAXD) evts[d[0]].push_back(3*k+0);
			if (d[1] < MAXD) evts[d[1]].push_back(3*k+1);
			if (d[0] < MAXD && d[1] < MAXD) evts[max(d[0], d[1])].push_back(3*k+2);
		}
	}

	vector<int> cnts(3*K);
	array<pair<int, int>, 3> tots{pair<int,int>{K,1},{K,1},{K,1}};

	vector<int> invs(MAXD);
	{
		int fact = 1;
		for (int i = 1; i <= MAXD-1; i++) {
			invs[i] = fact;
			fact = mul(fact, i);
		}
		fact = minv(fact, MOD);
		for (int i = MAXD-1; i >= 1; i--) {
			invs[i] = mul(invs[i], fact);
			fact = mul(fact, i);
		}
	}

	auto query = [&]() {
		return sub(sub(tot_val, tots[0].first ? 0 : tots[0].second), sub(tots[1].first ? 0 : tots[1].second, tots[2].first ? 0 : tots[2].second));
	};
	int ans = 0;
	for (int d = 0; d < MAXD; d++) {
		for (auto evt : evts[d]) {
			int v = evt % 3;
			if (!cnts[evt]) {
				tots[v].first--;
			} else {
				tots[v].second = mul(tots[v].second, invs[cnts[evt]]);
			}
			cnts[evt]++;
			tots[v].second = mul(tots[v].second, cnts[evt]);
		}
		ans = sub(ans, MOD-query());
	}
	ans = sub(ans, mul(query(), MAXD));
	cout << ans << '\n';
	return 0;
}
