#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N, M; cin >> N >> M;
	struct edge_t {
		int a, b, r, p;
	};
	vector<edge_t> edges(M);
	vector<vector<int>> inEdges(N);
	vector<vector<int>> outEdges(N);
	for (int e = 0; e < M; e++) {
		int a, b, r, p; cin >> a >> b >> r >> p; a--, b--;
		edges[e] = {a, b, r, p};
		outEdges[a].push_back(e);
		inEdges[b].push_back(e);
	}

	vector<int> res(N, -2);
	vector<int> outDeg(N);
	for (int i = 0; i < N; i++) outDeg[i] = int(outEdges[i].size());

	vector<bool> alive(M, true);

	{
		vector<int> q; q.reserve(N);
		for (int i = 0; i < N; i++) {
			if (!outDeg[i]) q.push_back(i);
		}
		for (int z = 0; z < int(q.size()); z++) {
			int cur = q[z];
			res[cur] = -1;
			for (int e : inEdges[cur]) {
				alive[e] = false;
				int nxt = edges[e].a;
				--outDeg[nxt];
				if (!outDeg[nxt]) q.push_back(nxt);
			}
		}
	}

	priority_queue<pair<int, int>> evts;
	for (int e = 0; e < M; e++) {
		if (!alive[e]) continue;
		evts.push({edges[e].r, e});
	}
	while (!evts.empty()) {
		auto [r, e] = evts.top(); evts.pop();
		if (!alive[e]) continue;
		alive[e] = false;
		int cur = edges[e].a;
		--outDeg[cur];
		if (!outDeg[cur]) {
			res[cur] = r;
			for (int f : inEdges[cur]) {
				if (!alive[f]) continue;
				if (r - edges[f].p > edges[f].r) {
					evts.push({r - edges[f].p, f});
				}
			}
		}
	}

	for (int i = 0; i < N; i++) {
		cout << res[i] << " \n"[i+1==N];
	}

	return 0;
}

// setmin(v[a], max(r + p, v[b]) - p)
// if v[b] <= r + p, then we can relax this edge once and never do it again
//
// If we have an SCC, then all v are <= max_edge_r
