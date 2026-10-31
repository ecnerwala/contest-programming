#include <bits/stdc++.h>

template <typename flow_t = int, typename tot_flow_t = flow_t>
struct Dinic {
	int N;
	std::vector<std::vector<int>> adj;
	struct edge_t {
		int dest;
		flow_t cap;
	};
	std::vector<edge_t> edges;

	std::vector<char> seen;

	explicit Dinic(int N_) : N(N_), adj(N) {}

	void add_edge(int from, int to, flow_t cap) {
		return add_bi_edge(from, to, cap, 0);
	}

	void add_bi_edge(int from, int to, flow_t cap, flow_t rev_cap) {
		assert(cap >= 0);
		assert(rev_cap >= 0);
		int e = int(edges.size());
		edges.emplace_back(edge_t{to, cap});
		edges.emplace_back(edge_t{from, rev_cap});
		adj[from].push_back(e);
		adj[to].push_back(e+1);
	}

	static constexpr tot_flow_t INF_FLOW = std::numeric_limits<tot_flow_t>::max() / 4;
	std::vector<int> buf;
	std::vector<int> level;
	tot_flow_t dinic_dfs(int cur, int t, tot_flow_t f) {
		if (cur == t) return f;
		tot_flow_t cur_f = 0;
		assert(f > 0);
		for (; buf[cur] < int(adj[cur].size()); buf[cur]++) {
			int e = adj[cur][buf[cur]];
			int nxt = edges[e].dest;
			if (level[nxt] == level[cur] + 1 && edges[e].cap > 0) {
				flow_t v = flow_t(dinic_dfs(nxt, t, std::min<tot_flow_t>(f, edges[e].cap)));
				edges[e].cap -= v;
				edges[e^1].cap += v;
				f -= v;
				cur_f += v;
				if (f == 0) break;
			}
		}
		return cur_f;
	}
	tot_flow_t dinic(int s, int t) {
		tot_flow_t tot_flow = 0;
		while (true) {
			buf.clear();
			buf.reserve(N);
			level.assign(N, -1);
			buf.push_back(s);
			level[s] = 0;
			for (int z = 0; z < int(buf.size()); z++) {
				int cur = buf[z];
				for (int e : adj[cur]) {
					int nxt = edges[e].dest;
					if (edges[e].cap > 0 && level[nxt] == -1) {
						level[nxt] = level[cur] + 1;
						buf.push_back(nxt);
					}
				}
			}
			if (level[t] == -1) break;
			buf.assign(N, 0);
			tot_flow += dinic_dfs(s, t, INF_FLOW);
		}
		return tot_flow;
	}
	tot_flow_t max_flow(int s, int t) { return dinic(s, t); }
};

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N1, N2, K, M; cin >> N1 >> N2 >> K >> M;
	std::vector<int> C(K);
	for (auto& c : C) cin >> c;

	struct edge_t {
		std::array<int, 2> vs;
		int w;
	};
	std::vector<edge_t> edges(M);
	for (auto& e : edges) {
		cin >> e.vs[0] >> e.vs[1] >> e.w; e.vs[0]--, e.vs[1]--;
		e.vs[1] += N1;
	}

	std::vector<std::vector<bool>> can(N1+N2, std::vector<bool>(K));
	for (int i = 0; i < N1+N2; i++) {
		int l; cin >> l;
		while (l--) {
			int k; cin >> k; k--;
			assert(!can[i][k]);
			can[i][k] = true;
		}
	}

	int64_t tot_cost = std::accumulate(C.begin(), C.end(), int64_t(0));

	const int64_t INF = 1e18;

	Dinic<int64_t> flower(2 + (N1 + N2) * K);
	const int S = 0, T = 1;
	for (int i = 0; i < N1; i++) {
		for (int k = 0; k < K; k++) {
			flower.add_edge(S, 2 + i*K + k, tot_cost * C[k]);
			if (!can[i][k]) {
				flower.add_edge(2 + i*K + k, T, INF);
			}
		}
	}
	for (int i = N1; i < N1+N2; i++) {
		for (int k = 0; k < K; k++) {
			flower.add_edge(2 + i*K + k, T, tot_cost * C[k]);
			if (!can[i][k]) {
				flower.add_edge(S, 2 + i*K + k, INF);
			}
		}
	}
	for (int i = 0; i < N1+N2; i++) {
		for (int k1 = 0; k1 < K; k1++) {
			for (int k2 = k1+1; k2 < K; k2++) {
				int64_t cost = int64_t(C[k1]) * int64_t(C[k2]);
				flower.add_bi_edge(2 + i*K + k1, 2 + i*K + k2, cost, cost);
			}
		}
	}

	for (auto e : edges) {
		auto [i, j] = e.vs;
		for (int k = 0; k < K; k++) {
			flower.add_edge(2 + i*K + k, 2 + j*K + k, e.w);
		}
	}

	int64_t ans = tot_cost * tot_cost * (N1 + N2) - flower.max_flow(S, T);
	cout << ans << '\n';

	return 0;
}
