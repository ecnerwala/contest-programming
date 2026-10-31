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

	auto sort_perm = [](std::vector<int> perm) -> std::vector<std::array<int, 2>> {
		int N = int(perm.size());
		std::vector<int> pts; pts.reserve(N);
		std::vector<std::array<int, 2>> ops; ops.reserve(N);
		for (int i = 0; i < N; i++) {
			while (perm[i] != i) {
				pts.clear();
				for (int cur = perm[i]; cur != i; cur = perm[cur]) pts.push_back(cur);
				std::sort(pts.begin(), pts.end());
				assert(perm[i] > i);
				int l = i;
				int r = -1;
				for (int p : pts) {
					if (perm[p] < p) {
						r = p;
						break;
					} else {
						l = p;
					}
				}
				assert(r != -1);
				assert(perm[l] > l);
				assert(perm[r] < r);
				assert(perm[l] >= r);
				assert(perm[r] <= l);
				ops.push_back({l, r});
				std::swap(perm[l], perm[r]);
			}
		}
		return ops;
	};

	int T; cin >> T;
	while (T--) {
		int N; cin >> N;
		std::vector<int> P(N);
		for (auto& a : P) { cin >> a; a--; }
		std::vector<int> Q(N);
		for (auto& a : Q) { cin >> a; a--; }

		MCMF_Dinic<int, int> flower(2*N+2);
		int SRC = 2*N, SNK = 2*N+1;
		for (int i = 0; i < N; i++) {
			flower.add_edge(SRC, i, 1, 0);
			flower.add_edge(i+N, SNK, 1, 0);
		}
		for (int i = 0; i < N; i++) {
			for (int j = 0; j < N; j++) {
				flower.add_edge(i, N+j, 1, std::abs(i - j) + std::abs(P[i] - Q[j]));
			}
		}
		auto [tot_flow, tot_cost] = flower.max_flow(SRC, SNK);
		assert(tot_flow == N);

		struct pt_t {
			int p_idx;
			int p_val;
			int q_idx;
			int q_val;
		};
		std::vector<pt_t> A; A.reserve(N);

		for (int i = 0; i < N; i++) {
			for (int j = 0; j < N; j++) {
				int e = 4 * N + 2 * (i * N + j);
				assert(flower.edges[e+0].dest == N+j);
				assert(flower.edges[e+1].dest == i);
				if (flower.edges[e].cap == 0) {
					// match i to j
					A.push_back({i, P[i], j, Q[j]});
				}
			}
		}
		assert(int(A.size()) == N);

		std::vector<std::array<int, 2>> all_ops; all_ops.reserve(2*N);
		{
			std::vector<int> perm(N, -1);
			for (int i = 0; i < N; i++) {
				perm[A[i].p_idx] = A[i].q_idx;
				A[i].p_idx = A[i].q_idx;
			}
			all_ops = sort_perm(perm);
		}
		{
			std::vector<int> perm(N, -1);
			std::vector<int> p_val_to_idx(N, -1);
			for (int i = 0; i < N; i++) {
				perm[A[i].p_val] = A[i].q_val;
				p_val_to_idx[A[i].p_val] = A[i].p_idx;
			}
			auto ops = sort_perm(perm);
			for (auto [i, j] : ops) {
				all_ops.push_back({ p_val_to_idx[i], p_val_to_idx[j] });
				std::swap(p_val_to_idx[i], p_val_to_idx[j]);
			}
		}

		cout << all_ops.size() << '\n';
		for (auto [i, j] : all_ops) {
			cout << i+1 << ' ' << j+1 << '\n';
		}
		std::vector<int> tmp_P = P;
		for (auto [i, j] : all_ops) {
			std::swap(tmp_P[i], tmp_P[j]);
		}
		for (int i = 0; i < N; i++) assert(tmp_P[i] == i);
	}

	return 0;
}
