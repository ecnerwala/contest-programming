#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N, M; cin >> N >> M;
	vector<int> P(N); for (auto& p : P) cin >> p;

	vector<vector<int>> inEdges(N);
	vector<int> outDeg(N);

	for (int e = 0; e < M; e++) {
		int u, v; cin >> u >> v; u--, v--;
		outDeg[u]++;
		inEdges[v].push_back(u);
	}

	vector<bool> bad(N);

	vector<int> q; q.reserve(N);
	for (int i = 0; i < N; i++) {
		if (!outDeg[i]) {
			bad[i] = true;
			q.push_back(i);
		}
	}

	for (int z = 0; z < int(q.size()); z++) {
		int cur = q[z];
		assert(bad[cur]);
		for (int nxt : inEdges[cur]) {
			if (bad[nxt]) continue;
			if (P[nxt] || !--outDeg[nxt]) {
				bad[nxt] = true;
				q.push_back(nxt);
			}
		}
	}

	for (int i = 0; i < N; i++) {
		cout << bad[i] << " \n"[i+1==N];
	}

	return 0;
}
