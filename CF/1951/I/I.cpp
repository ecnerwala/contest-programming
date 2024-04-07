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

	int T; cin >> T;
	while (T--) {
		int N, M, K; cin >> N >> M >> K;
		struct edge_t {
			std::array<int, 2> dest;
			int a, b;
		};
		std::vector<edge_t> edges(M);
		for (int e = 0; e < M; e++) {
			int u, v, a, b; cin >> u >> v >> a >> b; u--, v--;
			edges[e] = {{u, v}, a, b};
		}
		std::vector<int> cnts(M);
		auto check = [&]() {
			//cerr << "Check " << '\n';
			//for (int v : cnts) { cerr << v << ' '; } cerr << '\n';
			Dinic<int> flower(2+N+M);
			for (int i = 0; i < N; i++) {
				flower.add_edge(N+M, i, K);
			}
			int tot_cnts = 0;
			for (int e = 0; e < M; e++) {
				// why not
				if (cnts[e] > K) return false;
				tot_cnts += cnts[e];

				flower.add_edge(edges[e].dest[0], N+e, K);
				flower.add_edge(edges[e].dest[1], N+e, K);
				flower.add_edge(N+e, N+M+1, cnts[e]);
			}
			if (flower.max_flow(N+M, N+M+1) != tot_cnts) {
				return false;
			}

			Dinic<int> flower2(1+N);
			for (int i = 0; i < N; i++) {
				flower2.add_bi_edge(N, i, flower.edges[2*i+0].cap, flower.edges[2*i+1].cap);
			}
			for (int e = 0; e < M; e++) {
				flower2.add_bi_edge(
					edges[e].dest[0],
					edges[e].dest[1],
					flower.edges[2*(N+3*e+1)+1].cap,
					flower.edges[2*(N+3*e+0)+1].cap
				);
			}
			/*
			std::vector<int> costs(flower2.edges.size());
			for (int z = 0; z < int(flower2.edges.size()); z++) {
				costs[z] = flower2.edges[z].cap;
			}
			*/
			int cur_source = N;
			for (int i = 0; i < N; i++) {
				/*
				for (int z = 0; z < int(flower2.edges.size()); z++) {
					flower2.edges[z].cap = costs[z];
				}
				*/
				int f = flower2.max_flow(cur_source, i);
				assert(f <= K);
				if (f != K) {
					return false;
				}
				cur_source = i;
			}
			return true;
		};

		int num_cap = 0;
		std::vector<int> cap(M, -1);
		// The ith edge costs (b[i] + a[i] (2i-1)) * M + e
		const int64_t INF_COST = M * 4 * int64_t(K+1) * 1000;
		int64_t mi = 0;
		int64_t ma = INF_COST;
		while (num_cap < M) {
			int64_t max_take = (mi + ma) / 2;
			// find the first edge to delete
			int64_t next_edge = ma;
			int64_t prev_edge = mi + 1;
			for (int e = 0; e < M; e++) {
				if (max_take < e) {
					cnts[e] = 0;
				} else {
					int64_t c = (max_take - e) / M;
					if (c < edges[e].a + edges[e].b) {
						cnts[e] = 0;
					} else {
						c -= edges[e].b;
						c /= edges[e].a;
						c++;
						c /= 2;
						if (c > K) c = K+1;

						cnts[e] = int(c);
					}
				}
				if (cap[e] != -1) {
					assert(cnts[e] >= cap[e]);
					cnts[e] = cap[e];
				}
				int64_t take_edge;
				if (cnts[e] == 0) {
					take_edge = 0;
				} else {
					take_edge = (edges[e].b + edges[e].a * int64_t(2 * cnts[e] - 1)) * M + e;
				}
				prev_edge = std::max(prev_edge, take_edge);
				int64_t no_take_edge;
				if (cap[e] != -1) {
					no_take_edge = INF_COST;
				} else {
					no_take_edge = (edges[e].b + edges[e].a * int64_t(2 * (cnts[e] + 1) - 1)) * M + e;
				}
				next_edge = std::min(next_edge, no_take_edge);
			}
			if (check()) {
				// we're good
				mi = next_edge - 1;
			} else {
				ma = prev_edge;
			}
			if (ma - mi == 1) {
				// Then the thing with cost exactly ma is bad
				int e = int(ma % M);
				//cerr << "bad cost " << ma << ' ' << e << ' ' << ma / M << '\n';
				int64_t c = (ma - e) / M;
				assert(c >= edges[e].a + edges[e].b);
				c -= edges[e].b;
				c /= edges[e].a;
				c--;
				c /= 2;
				//cerr << "cap " << e << ' ' << c << '\n';
				assert(cap[e] == -1);
				cap[e] = int(c);
				num_cap++;
				// Now, reset the things
				mi = ma;
				ma = INF_COST;
			}
		}
		int64_t ans = 0;
		for (int e = 0; e < M; e++) {
			ans += int64_t(cap[e]) * (int64_t(edges[e].a) * int64_t(cap[e]) + edges[e].b);
		}
		cout << ans << '\n';
	}

	return 0;
}
