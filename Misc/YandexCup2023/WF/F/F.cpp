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
			int cur_flow = dinic(s, t);
			tot_flow += cur_flow;
			tot_cost += cur_flow * pi[t];
		}
		return {tot_flow, tot_cost};
	}
};

namespace std {

template<class Fun>
class y_combinator_result {
	Fun fun_;
public:
	template<class T>
	explicit y_combinator_result(T &&fun): fun_(std::forward<T>(fun)) {}

	template<class ...Args>
	decltype(auto) operator()(Args &&...args) {
		return fun_(std::ref(*this), std::forward<Args>(args)...);
	}
};

template<class Fun>
decltype(auto) y_combinator(Fun &&fun) {
	return y_combinator_result<std::decay_t<Fun>>(std::forward<Fun>(fun));
}

} // namespace std

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N, M, K; cin >> N >> M >> K;
	MCMF_Dinic<int, int> flower(2*N+2);

	flower.edges.reserve(2 * N + 2 * M);
	for (int i = 0; i < N; i++) {
		flower.add_edge(2*N, i, 1, 0);
	}
	for (int i = 0; i < N; i++) {
		flower.add_edge(N+i, 2*N+1, K, 0);
	}
	for (int e = 0; e < M; e++) {
		int u, v; cin >> u >> v; u--, v--;
		flower.add_edge(u, N+v, 1, 0);
		flower.add_edge(v, N+u, 1, 0);
	}
	flower.dinic(2*N, 2*N+1);

	std::vector<int> out_edge(N, -1);
	std::vector<int> in_deg(N, 0);
	for (int i = 0; i < N; i++) {
		for (int e : flower.adj[i]) {
			int d = flower.edges[e].dest;
			if (d == 2 * N) continue;
			assert(N <= d && d < 2 * N);
			if (flower.edges[e].cap == 0) {
				assert(out_edge[i] == -1);
				out_edge[i] = d - N;
				in_deg[d-N] ++;
			}
		}
	}

	// Our graph is a partial functional graph, so we have a collection of
	// trees and jellyfish. We will maintain the property that every vertex
	// with an outedge will be used. First, use all vertices with indegree 0.

	std::vector<std::vector<int>> ch(N);
	std::vector<int> q; q.reserve(N);
	for (int i = 0; i < N; i++) {
		if (in_deg[i] == 0 && out_edge[i] != -1) q.push_back(i);
	}
	for (int z = 0; z < int(q.size()); z++) {
		int cur = q[z];
		assert(in_deg[cur] == 0);
		assert(out_edge[cur] != -1);
		int nxt = out_edge[cur];
		ch[nxt].push_back(cur);
		int nxt_nxt = std::exchange(out_edge[nxt], -1);
		if (nxt_nxt != -1) {
			if (--in_deg[nxt_nxt] == 0) {
				if (out_edge[nxt_nxt] != -1) {
					q.push_back(nxt_nxt);
				}
			}
		}
	}

	// Now, we're only left with isolated cycles.

	std::vector<int> cyc; cyc.reserve(N);
	for (int i = 0; i < N; i++) {
		if (in_deg[i] != 0 && out_edge[i] != -1) {
			assert(in_deg[i] == 1);
			// Now, let's solve the cycle
			for (int cur = i; out_edge[cur] != -1; ) {
				cyc.push_back(cur);
				in_deg[out_edge[cur]]--;
				cur = std::exchange(out_edge[cur], -1);
			}
			assert(int(cyc.size()) >= 2);
			for (int z = 0; z+1 < int(cyc.size()); z += 2) {
				ch[cyc[z+1]].push_back(cyc[z]);
			}
			if (int(cyc.size()) & 1) {
				ch[cyc.end()[-2]].push_back(cyc.end()[-1]);
			}
			cyc.clear();
		}
	}

	int num_ans = 0;
	for (int i = 0; i < N; i++) if (!ch[i].empty()) num_ans++;
	cout << num_ans << '\n';
	for (int i = 0; i < N; i++) if (!ch[i].empty()) {
		cout << ch[i].size() << ' ' << i+1;
		for (int v : ch[i]) {
			cout << ' ' << v+1;
		}
		cout << '\n';
	}

	return 0;
}
