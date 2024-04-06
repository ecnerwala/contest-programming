#include <bits/stdc++.h>
#include <ext/pb_ds/priority_queue.hpp>

template <typename flow_t = int>
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
		assert(cap >= 0);
		int e = int(edges.size());
		edges.emplace_back(edge_t{to, cap});
		edges.emplace_back(edge_t{from, 0});
		adj[from].push_back(e);
		adj[to].push_back(e+1);
	}

	static constexpr flow_t INF_FLOW = std::numeric_limits<flow_t>::max() / 4;
	std::vector<int> buf;
	std::vector<int> level;
	flow_t dinic_dfs(int cur, int t, flow_t f) {
		if (cur == t) return f;
		flow_t cur_f = 0;
		assert(f > 0);
		for (; buf[cur] < int(adj[cur].size()); buf[cur]++) {
			int e = adj[cur][buf[cur]];
			int nxt = edges[e].dest;
			if (level[nxt] == level[cur] + 1 && edges[e].cap > 0) {
				flow_t v = dinic_dfs(nxt, t, std::min(f, edges[e].cap));
				edges[e].cap -= v;
				edges[e^1].cap += v;
				f -= v;
				cur_f += v;
				if (f == 0) break;
			}
		}
		return cur_f;
	}
	flow_t dinic(int s, int t) {
		flow_t tot_flow = 0;
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
	flow_t max_flow(int s, int t) { return dinic(s, t); }
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
				flower.add_edge(N+M, i, K, 0);
			}
			int tot_cnts = 0;
			for (int e = 0; e < M; e++) {
				// why not
				if (cnts[e] > K) return false;
				tot_cnts += cnts[e];

				flower.add_edge(edges[e].dest[0], N+e, K, 0);
				flower.add_edge(edges[e].dest[1], N+e, K, 0);
				flower.add_edge(N+e, N+M+1, cnts[e], 0);
			}
			if (flower.max_flow(N+M, N+M+1) != tot_cnts) {
				return false;
			}
			int cur_source = N+M;
			for (int i = 0; i < N; i++) {
				int f = flower.max_flow(cur_source, i);
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
