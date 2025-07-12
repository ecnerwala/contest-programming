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

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N; cin >> N;
		std::vector<int> S(N);
		{
			std::string S_; S_.reserve(N); cin >> S_;
			assert(int(S_.size()) == N);
			for (int i = 0; i < N; i++) {
				char c = S_[i];
				if ('a' <= c && c <= 'z') S[i] = c - 'a';
				else if ('A' <= c && c <= 'Z') S[i] = 26 + (c - 'A');
				else assert(false);
			}
		}
		std::array<int, 52> freq; freq.fill(0);
		for (int c : S) freq[c]++;

		auto solve_matching = [&](const std::vector<std::vector<int64_t>>& v) -> int64_t {
			int L = int(v.size());
			MCMF_Dinic<int, int64_t> flower(2*L+2);
			for (int i = 0; i < L; i++) {
				flower.add_edge(0, 2 + i, 1, 0);
				flower.add_edge(2 + L + i, 1, 1, 0);
			}
			for (int i = 0; i < L; i++) {
				for (int j = 0; j < L; j++) {
					flower.add_edge(2 + i, 2 + L+i, 1, v[i][j]);
				}
			}
			auto [flow, cost] = flower.max_flow(0, 1);
			assert(flow == L);
			return cost;
		};

		auto solve_r = [&](int r) -> std::optional<int> {
			// we need to bucket things into groups of N / r
			int small_sz = N / r;
			int big_sz = small_sz + 1;

			int need_big = N % r;
			int need_small = r - need_big;

			{
				int tot_big = 0;
				int tot_small = 0;
				for (int f : freq) {
					// more bigs is better, cause it's just a smaller r
					int num_small = (big_sz - f % big_sz) % big_sz;
					int num_big = (f + num_small) / big_sz - num_small;
					if (num_big < 0) return std::nullopt;

					tot_big += num_big;
					tot_small += num_small;
				}
				if (tot_big < need_big) return std::nullopt;
				assert(tot_big == need_big && tot_small == need_small);
			}

			std::array<std::vector<int>, 52> inds;
			for (int v = 0; v < 52; v++) inds[v].reserve(freq[v]);
			for (int i = 0; i < N; i++) {
				inds[S[i]].push_back(i);
			}

			std::vector<std::vector<int64_t>> big_weights(need_big, std::vector<int64_t>(need_big));
			std::vector<std::vector<int64_t>> small_weights(need_small, std::vector<int64_t>(need_small));
			int big_idx = 0;
			int small_idx = 0;
			for (int v = 0; v < 52; v++) {
				int f = freq[v];

				// more bigs is better, cause it's just a smaller r
				int num_small = (big_sz - f % big_sz) % big_sz;
				int num_big = (f + num_small) / big_sz - num_small;
				if (num_big < 0) return std::nullopt;

				for (int i = 0; i < num_big; i++) {
					for (int m = 0, z = i; m < big_sz; m++, z += num_big + num_small) {
						int loc = inds[v][z];
						for (int a = 0; a < need_big; a++) {
							big_weights[big_idx][a] += std::abs(m * r + a - loc);
						}
					}
					big_idx++;
				}

				for (int i = 0; i < num_small; i++) {
					for (int m = 0, z = num_big + i; m < small_sz; m++, z += num_big + num_small) {
						int loc = inds[v][z];
						for (int a = 0; a < need_small; a++) {
							small_weights[small_idx][a] += std::abs(m * r + (need_big + a) - loc);
						}
					}
					small_idx++;
				}
			}
			assert(big_idx == need_big);
			assert(small_idx == need_small);

			return solve_matching(big_weights) + solve_matching(small_weights);
		};

		for (int r = 1; r <= N; r++) {
			auto a_opt = solve_r(r);
			if (a_opt) {
				cout<< r << ' ' << *a_opt << '\n';
				break;
			}
		}
	}

	return 0;
}
