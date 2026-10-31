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
	Dinic<int> flower(2*N+2);

	flower.edges.reserve(2 * N + 2 * M);
	for (int i = 0; i < N; i++) {
		flower.add_edge(2*N, i, 1);
	}
	for (int i = 0; i < N; i++) {
		flower.add_edge(N+i, 2*N+1, K);
	}
	for (int e = 0; e < M; e++) {
		int u, v; cin >> u >> v; u--, v--;
		flower.add_edge(u, N+v, 1);
		flower.add_edge(v, N+u, 1);
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
