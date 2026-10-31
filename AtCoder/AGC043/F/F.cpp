#include<bits/stdc++.h>
#include<bits/extc++.h>
using namespace std;

template <typename flow_t = int, typename cost_t = int64_t>
struct MCMF_SSPA {
	int N;
	vector<vector<int>> adj;
	struct edge_t {
		int dest;
		flow_t cap;
		cost_t cost;
	};
	vector<edge_t> edges;

	vector<char> seen;
	vector<cost_t> pi;
	vector<int> prv;

	explicit MCMF_SSPA(int N_) : N(N_), adj(N), pi(N, 0), prv(N) {}

	void addVert() {
		N++;
		adj.emplace_back();
		pi.push_back(0);
		prv.emplace_back();
	}

	void addEdge(int from, int to, flow_t cap, cost_t cost) {
		assert(cap >= 0);
		int e = int(edges.size());
		edges.emplace_back(edge_t{to, cap, cost});
		edges.emplace_back(edge_t{from, 0, -cost});
		adj[from].push_back(e);
		adj[to].push_back(e+1);
	}

	const cost_t INF_COST = numeric_limits<cost_t>::max() / 4;
	const flow_t INF_FLOW = numeric_limits<flow_t>::max() / 4;
	vector<cost_t> dist;
	__gnu_pbds::priority_queue<pair<cost_t, int>> q;
	vector<typename decltype(q)::point_iterator> its;
	void path(int s) {
		dist.assign(N, INF_COST);
		dist[s] = 0;

		its.assign(N, q.end());
		its[s] = q.push({0, s});

		while (!q.empty()) {
			int i = q.top().second; q.pop();
			cost_t d = dist[i];
			for (int e : adj[i]) {
				if (edges[e].cap) {
					int j = edges[e].dest;
					cost_t nd = d + edges[e].cost;
					if (nd < dist[j]) {
						dist[j] = nd;
						prv[j] = e;
						if (its[j] == q.end()) {
							its[j] = q.push({-(dist[j] - pi[j]), j});
						} else {
							q.modify(its[j], {-(dist[j] - pi[j]), j});
						}
					}
				}
			}
		}

		swap(pi, dist);
	}

	vector<pair<cost_t, flow_t>> maxflow(int s, int t) {
		assert(s != t);
		vector<pair<cost_t, flow_t>> res;
		while (path(s), pi[t] < INF_COST) {
			flow_t curFlow = numeric_limits<flow_t>::max();
			for (int cur = t; cur != s; ) {
				int e = prv[cur];
				int nxt = edges[e^1].dest;
				curFlow = min(curFlow, edges[e].cap);
				cur = nxt;
			}
			if (!res.empty() && res.back().first == pi[t]) {
				res.back().second += curFlow;
			} else {
				res.emplace_back(pi[t], curFlow);
			}
			for (int cur = t; cur != s; ) {
				int e = prv[cur];
				int nxt = edges[e^1].dest;
				edges[e].cap -= curFlow;
				edges[e^1].cap += curFlow;
				cur = nxt;
			}
		}
		return res;
	}
};

using i64 = int64_t;

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	int N; cin >> N;

	struct jewel {
		int P;
		i64 S, C;
	};
	vector<vector<jewel>> jewels(N);

	for (int i = 0; i < N; i++) {
		int K; cin >> K;
		jewels[i].resize(K);
		for (int j = 0; j < K; j++) {
			cin >> jewels[i][j].S >> jewels[i][j].P >> jewels[i][j].C;
		}
		sort(jewels[i].begin(), jewels[i].end(), [&](const jewel& a, const jewel& b) -> bool { return a.S < b.S; });
	}

	int V = 3;
	vector<vector<int>> verts(N);
	for (int i = 0; i < N; i++) {
		verts[i].push_back(0);
		for (int j = 1; j < int(jewels[i].size()); j++) {
			verts[i].push_back(V++);
		}
		verts[i].push_back(2);
	}

	MCMF_SSPA<int, i64> mcmf(V);

	int totP = 0;
	for (int i = 0; i < N; i++) {
		int K = int(jewels[i].size());
		for (int j = 0; j < K; j++) {
			totP += jewels[i][j].P;
		}
	}

	// limit it by totP + 2
	int infFlow = totP + 2;
	mcmf.addEdge(2, 1, infFlow, 0);

	for (int i = 0; i < N; i++) {
		int K = int(jewels[i].size());
		for (int j = 0; j < K; j++) {
			mcmf.addEdge(verts[i][j], verts[i][j+1], jewels[i][j].P, 0);
			mcmf.addEdge(verts[i][j+1], verts[i][j], infFlow, 0);
			mcmf.addEdge(verts[i][j], verts[i][j+1], infFlow, jewels[i][j].C);
		}
	}

	int M; cin >> M;
	for (int e = 0; e < M; e++) {
		int u, v; i64 w; cin >> u >> v >> w; u--, v--;
		for (int j = 0; j < int(jewels[u].size()); j++) {
			int k = 0;
			while (k < int(jewels[v].size()) && jewels[v][k].S <= jewels[u][j].S + w) {
				k++;
			}
			mcmf.addEdge(verts[v][k], verts[u][j+1], infFlow, 0);
		}
	}

	auto flows = mcmf.maxflow(0, 1);
	//for (auto it : flows) { cerr << it.first << ' ' << it.second << '\n'; }

	vector<pair<i64, i64>> prefFlows(flows.size()+1);
	prefFlows[0] = {0,0};
	for (int i = 0; i < int(flows.size()); i++) {
		prefFlows[i+1] = {prefFlows[i].first + flows[i].first * flows[i].second, prefFlows[i].second + flows[i].second};
	}
	assert(prefFlows.back().second == infFlow);

	int Q; cin >> Q;
	while (Q--) {
		i64 A; cin >> A;
		int i = int(lower_bound(flows.begin(), flows.end(), pair<i64, int>(A, -1)) - flows.begin());
		if (i == int(flows.size())) {
			cout << -1 << '\n';
		} else {
			cout << A * prefFlows[i].second - prefFlows[i].first << '\n';
		}
	}

	return 0;
}
