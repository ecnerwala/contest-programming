#include<bits/stdc++.h>
#include <ext/pb_ds/priority_queue.hpp>

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

	int NUM_CASES; cin >> NUM_CASES;
	for (int case_num = 1; case_num <= NUM_CASES; case_num ++) {

		int R, C; cin >> R >> C;
		std::vector<std::string> G(R);
		for (auto& s : G) { cin >> s; assert(int(s.size()) == C); }

		cerr << "Case #" << case_num << " " << R << 'x' << C << '\n';

		auto is_good = [&](const std::string& cnd) -> bool {
			for (int st = 0; st < R+C; st++) {
				Dinic<int> flower(2 * (R*C) + 2);
				int S = 2 * (R*C);
				int T = 2 * (R*C) + 1;
				int expected = 0;
				for (int i = 0; i < R; i++) {
					for (int j = 0; j < C; j++) {
						char v = G[i][j];
						bool is_upper = ('A' <= v && v <= 'Z');
						if (is_upper) v = v - 'A' + 'a';
						if (i + j < st) continue;
						int idx = i + j - st;
						if (idx >= int(cnd.size())) continue;
						if (is_upper && idx == 0) {
							flower.add_edge(S, (i*C+j) * 2 + 0, 1);
							expected++;
						}
						if (v > cnd[idx] || (idx + 1 == int(cnd.size()) && v == cnd[idx])) {
							//flower.add_edge((i*C+j) * 2 + 0, (i*C+j) * 2 + 1, 1);
							flower.add_edge((i*C+j) * 2 + 0, T, 1);
						} else if (v == cnd[idx]) {
							flower.add_edge((i*C+j) * 2 + 0, (i*C+j) * 2 + 1, 1);
						}
					}
				}
				for (int i = 0; i < R; i++) {
					for (int j = 0; j < C; j++) {
						if (i + 1 < R) {
							flower.add_edge((i*C+j) * 2 + 1, ((i+1)*C+j) * 2 + 0, 1);
						}
						if (j + 1 < C) {
							flower.add_edge((i*C+j) * 2 + 1, (i*C+(j+1)) * 2 + 0, 1);
						}
					}
				}
				if (flower.max_flow(S, T) != expected) return false;
			}
			return true;
		};
		std::string ans;
		while (true) {
			char mi = 'a' - 1;
			char ma = 'z' + 1;
			while (ma - mi > 1) {
				char md = mi + (ma - mi) / 2;
				if (is_good(ans + md)) {
					mi = md;
				} else {
					ma = md;
				}
			}
			if (mi == 'a' - 1) {
				break;
			} else {
				ans += mi;
			}
		}

		cout << "Case #" << case_num << ": " << ans << '\n';;
	}

	return 0;
}
