#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N, M; cin >> N >> M;
	vector<vector<int>> outEdges(N);
	vector<vector<int>> inEdges(N);
	vector<int> inDeg(N);
	for (int e = 0; e < M; e++) {
		int u, v; cin >> u >> v; u--, v--;
		outEdges[u].push_back(v);
		inEdges[v].push_back(u);
		inDeg[v]++;
	}

	vector<int> toposort; toposort.reserve(N);
	for (int i = 0; i < N; i++) {
		if (!inDeg[i]) toposort.push_back(i);
	}
	vector<int> topoIdx(N);
	for (int z = 0; z < N; z++) {
		assert(z < int(toposort.size()));
		int cur = toposort[z];
		topoIdx[cur] = z;
		for (int nxt : outEdges[cur]) {
			if (!--inDeg[nxt]) toposort.push_back(nxt);
		}
	}

	vector<int> pref(N+1);
	pref[0] = 0;
	for (int i = 0; i < N; i++) {
		int canReach = N;
		for (int v : outEdges[toposort[i]]) {
			canReach = min(canReach, topoIdx[v]);
		}
		pref[i+1] = max(pref[i], canReach);
	}
	vector<int> suff(N+1);
	suff[N] = N-1;
	for (int i = N-1; i >= 0; i--) {
		int canReach = -1;
		for (int v : inEdges[toposort[i]]) {
			canReach = max(canReach, topoIdx[v]);
		}
		suff[i] = min(suff[i+1], canReach);
	}
	int ans = 0;
	for (int i = 0; i < N; i++) {
		if (pref[i] == i && suff[i+1] == i) {
			ans++;
		}
	}
	cout << ans << '\n';

	return 0;
}
