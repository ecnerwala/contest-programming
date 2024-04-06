#include <bits/stdc++.h>
#include <ext/pb_ds/priority_queue.hpp>

template <typename flow_t = int, typename cost_t = int64_t>
struct MCMF_Dinic {
	int N;
	std::vector<std::vector<int>> adj;
	struct edge_t {
		int dest;
		flow_t cap;
		cost_t cost;
	};
	std::vector<edge_t> edges;

	std::vector<char> seen;
	std::vector<cost_t> pi;

	explicit MCMF_Dinic(int N_) : N(N_), adj(N), pi(N, 0) {}

	void add_edge(int from, int to, flow_t cap, cost_t cost) {
		assert(cap >= 0);
		assert(cost + pi[from] - pi[to] >= 0); // TODO: Remove this restriction
		int e = int(edges.size());
		edges.emplace_back(edge_t{to, cap, cost});
		edges.emplace_back(edge_t{from, 0, -cost});
		adj[from].push_back(e);
		adj[to].push_back(e+1);
	}

	static constexpr cost_t INF_COST = std::numeric_limits<cost_t>::max() / 4;
	static constexpr flow_t INF_FLOW = std::numeric_limits<flow_t>::max() / 4;
	std::vector<cost_t> dist;
	__gnu_pbds::priority_queue<std::pair<cost_t, int>> q;
	std::vector<typename decltype(q)::point_iterator> its;
	cost_t dijkstra(int s, int t) {
		dist.assign(N, INF_COST);
		dist[s] = 0;

		its.assign(N, q.end());
		its[s] = q.push({-(dist[s] - pi[s]), s});

		while (!q.empty()) {
			int i = q.top().second; q.pop();
			cost_t d = dist[i];
			for (int e : adj[i]) {
				if (edges[e].cap) {
					int j = edges[e].dest;
					cost_t nd = d + edges[e].cost;
					if (nd < dist[j]) {
						dist[j] = nd;
						if (its[j] == q.end()) {
							its[j] = q.push({-(dist[j] - pi[j]), j});
						} else {
							q.modify(its[j], {-(dist[j] - pi[j]), j});
						}
					}
				}
			}
		}

		std::swap(pi, dist);
		return pi[t];
	}

	std::vector<int> buf;
	std::vector<int> level;
	flow_t dinic_dfs(int cur, int t, flow_t f) {
		if (cur == t) return f;
		flow_t cur_f = 0;
		assert(f > 0);
		for (; buf[cur] < int(adj[cur].size()); buf[cur]++) {
			int e = adj[cur][buf[cur]];
			int nxt = edges[e].dest;
			if (level[nxt] == level[cur] + 1 && edges[e].cap > 0 && edges[e].cost == pi[nxt] - pi[cur]) {
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
					if (edges[e].cap > 0 && edges[e].cost == pi[nxt] - pi[cur] && level[nxt] == -1) {
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

	std::vector<std::pair<flow_t, cost_t>> all_flows(int s, int t, cost_t max_cost = INF_COST - 1) {
		assert(s != t);
		std::vector<std::pair<flow_t, cost_t>> res;
		while (dijkstra(s, t) <= max_cost) {
			assert(res.empty() || pi[t] > res.back().second);
			flow_t f = dinic(s, t);
			res.push_back({f, pi[t]});
		}
		return res;
	}

	std::pair<flow_t, cost_t> max_flow(int s, int t, cost_t max_cost = INF_COST - 1) {
		assert(s != t);
		flow_t tot_flow = 0; cost_t tot_cost = 0;
		while (dijkstra(s, t) <= max_cost) {
			flow_t cur_flow = dinic(s, t);
			tot_flow += cur_flow;
			tot_cost += cur_flow * pi[t];
		}
		return {tot_flow, tot_cost};
	}
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
			for (int z = 0; z < N; z++) {
				MCMF_Dinic<int, int> flower(3+N+M);
				for (int i = 0; i < N; i++) {
					if (i != z) {
						flower.add_edge(N+M, i, K, 0);
					}
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
				if (flower.max_flow(N+M+2, N+M+1).first != tot_cnts) {
					return false;
				}
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
				cerr << "bad cost " << ma << ' ' << e << ' ' << ma / M << '\n';
				int64_t c = (ma - e) / M;
				assert(c >= edges[e].a + edges[e].b);
				c -= edges[e].b;
				c /= edges[e].a;
				c--;
				c /= 2;
				cerr << "cap " << e << ' ' << c << '\n';
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
