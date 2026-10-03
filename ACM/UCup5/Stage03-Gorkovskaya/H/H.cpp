#include <bits/stdc++.h>
#include <cassert>
#include <ext/pb_ds/priority_queue.hpp>

namespace wala {

// NOTE: This doesn't support negative-cost edges; you can adjust edge weights
// (e.g. by precomputing a potential function) to make them positive.

template <typename flow_t = int, typename cost_t = int64_t>
struct MCMF_SSPA {
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
	std::vector<int> prv;

	explicit MCMF_SSPA(int N_) : N(N_), adj(N), pi(N, 0), prv(N) {}

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
		return pi[t];
	}

	flow_t path(int s, int t) {
		flow_t cur_flow = std::numeric_limits<flow_t>::max();
		for (int cur = t; cur != s; ) {
			int e = prv[cur];
			int nxt = edges[e^1].dest;
			cur_flow = std::min(cur_flow, edges[e].cap);
			cur = nxt;
		}
		for (int cur = t; cur != s; ) {
			int e = prv[cur];
			int nxt = edges[e^1].dest;
			edges[e].cap -= cur_flow;
			edges[e^1].cap += cur_flow;
			cur = nxt;
		}
		return cur_flow;
	}

	std::vector<std::pair<flow_t, cost_t>> all_flows(int s, int t, cost_t max_cost = INF_COST - 1) {
		assert(s != t);
		std::vector<std::pair<flow_t, cost_t>> res;
		while (dijkstra(s, t) <= max_cost) {
			assert(res.empty() || pi[t] >= res.back().second);
			flow_t f = path(s, t);
			res.push_back({f, pi[t]});
		}
		return res;
	}

	std::pair<flow_t, cost_t> max_flow(int s, int t, cost_t max_cost = INF_COST - 1) {
		assert(s != t);
		flow_t tot_flow = 0; cost_t tot_cost = 0;
		while (dijkstra(s, t) <= max_cost) {
			flow_t cur_flow = path(s, t);
			tot_flow += cur_flow;
			tot_cost += cur_flow * pi[t];
		}
		return {tot_flow, tot_cost};
	}
};

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

} // namespace wala

int main() {
	std::ios_base::sync_with_stdio(false), std::cin.tie(nullptr);

	auto slow = [&](int W, std::vector<int> freq) -> int {
		std::vector<int> A; A.reserve(W);
		for (int v = 1; v <= 7; v++) {
			for (int z = 0; z < freq[v]; z++) {
				A.push_back(v);
			}
		}
		assert(int(A.size()) == W);
		int ans = 0;
		do {
			int v = 0;
			for (int i = 0; i+1 < W; i++) v += std::lcm(A[i], A[i+1]);
			ans = std::max(ans, v);
		} while (std::next_permutation(A.begin(), A.end()));
		return ans;
	};
	auto solve = [&](int W, std::vector<int> freq) -> int {
		std::vector<std::pair<int, int>> vals; vals.reserve(7);
		for (int v = 1; v <= 7; v++) {
			if (freq[v]) vals.push_back({v, freq[v]});
		}

		int N = int(vals.size());

		assert(N >= 1);
		if (N == 1) {
			return vals[0].first * (vals[0].second - 1);
		}

		int ans = 0;

		// We pick a degree sequence, then pick a tree with that degree sequence, then pick a root and one edge around that root to ignore
		std::vector<int> prufer(N-2, 0);
		do {
			// current thing has some fixed degree
			std::vector<int> best_vals(N, 0);
			do {
				std::vector<int> deg(N, 1);
				for (auto v : prufer) deg[v]++;

				int cur_val = 0;
				std::vector<int> best_root(N, 43);
				// find the last leaf
				for (int z = 0; z < N-2; z++) {
					int a = 0;
					while (deg[a] != 1) a++;
					int b = prufer[z];
					deg[a]--, deg[b]--;
					int v = std::lcm(vals[a].first, vals[b].first);
					cur_val += v;
					best_root[a] = std::min(best_root[a], v);
					best_root[b] = std::min(best_root[b], v);
				}
				for (int i = 0; i < N; i++) {
					best_vals[i] = std::max(best_vals[i], cur_val - best_root[i]);
				}
			} while (std::next_permutation(prufer.begin(), prufer.end()));
			{
				std::vector<int> deg(N, 1);
				for (auto v : prufer) deg[v]++;
				for (int rt = 0; rt < N; rt++) {
					bool good = true;
					wala::MCMF_Dinic<int, int> flower(2 * N + 2);
					for (int i = 0; i < N; i++) {
						int tree_out = (i != rt);
						int tree_in = deg[i] - tree_out;
						if (tree_out > vals[i].second || tree_in > vals[i].second) {
							good = false;
							break;
						}
						flower.add_edge(0, 2 + i, vals[i].second - tree_in, 0);
						flower.add_edge(2 + N + i, 1, vals[i].second - tree_out, 0);
					}
					if (!good) continue;
					for (int i = 0; i < N; i++) {
						for (int j = 0; j < N; j++) {
							flower.add_edge(2 + i, 2 + N + j, W, 43 - std::lcm(vals[i].first, vals[j].first));
						}
					}
					auto [flow, cost] = flower.max_flow(0, 1);
					assert(flow == W - (N-1));
					std::cerr << rt << ' ' << best_vals[rt] << ' ' << 43 * flow - cost << '\n';
					ans = std::max(ans, best_vals[rt] + (43 * flow - cost));
				}
			}
		} while ([&]() -> bool {
			// Increment prufer to the lex next increasing sequence
			int z = N-3;
			while (z >= 0 && prufer[z] == N-1) z--;
			if (z == -1) return false;
			int v = prufer[z] + 1;
			for (; z < N-2; z++) prufer[z] = v;
			return true;
		}());
		return ans;
	};

	std::mt19937 mt(48);
	while (true) {
		int W = std::uniform_int_distribution<int>(1, 3)(mt);
		std::cerr << "test " << W << '\n';
		std::vector<int> freq(8);
		for (int i = 0; i < W; i++) freq[std::uniform_int_distribution<int>(1, 7)(mt)]++;
		auto slo = slow(W, freq);
		auto ans = solve(W, freq);
		if (slo != ans) {
			for (auto v : freq) { std::cerr << v << ' '; } std::cerr << '\n';
			std::cerr << "slo " << slo << '\n';
			std::cerr << "ans " << ans << '\n';
			assert(false);
		}
	}

	int T; std::cin >> T;
	while (T--) {
		int W; std::cin >> W;
		std::vector<int> freq(8);
		for (int i = 0; i < W; i++) {
			int a; std::cin >> a;
			freq[a]++;
		}
		std::cout << solve(W, freq) << '\n';
	}

	return 0;
}
