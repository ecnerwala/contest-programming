#include <bits/stdc++.h>

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

	vector<array<int, 4>> seg(2*K);

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

		seg[K+k].fill(0);

		for (int i = 0; i < N; i++) {
			array<int, 2> d{dist[2*i], dist[2*i+1]};
			assert(min(d[0], d[1]) < MAXD);

			seg[K+k][0]++;

			if (d[0] < MAXD) evts[d[0]].push_back(4*k+1);
			if (d[1] < MAXD) evts[d[1]].push_back(4*k+2);
			if (d[0] < MAXD && d[1] < MAXD) evts[max(d[0], d[1])].push_back(4*k+3);
		}
	}

	for (int a = K-1; a; a--) {
		for (int v = 0; v < 4; v++) seg[a][v] = mul(seg[2*a][v], seg[2*a+1][v]);
	}
	auto query = [&]() {
		return sub(sub(seg[1][0], seg[1][1]), sub(seg[1][2], seg[1][3]));
	};
	int ans = 0;
	for (int d = 0; d < MAXD; d++) {
		for (auto evt : evts[d]) {
			int k = evt >> 2;
			int v = evt & 3;
			seg[K+k][v]++;
			for (int a = (K+k)/2; a; a >>= 1) {
				seg[a][v] = mul(seg[2*a][v], seg[2*a+1][v]);
			}
		}
		ans = sub(ans, MOD-query());
	}
	ans = sub(ans, mul(query(), MAXD));
	cout << ans << '\n';
	return 0;
}
