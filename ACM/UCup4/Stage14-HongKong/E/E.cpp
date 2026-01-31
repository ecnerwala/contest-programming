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
		int N, M, Q; cin >> N >> M >> Q;
		
		Dinic base_flower(2*N+2);
		const int st = 0, en = 1;
		for (int e = 0; e < M; e++) {
			int u, v; cin >> u >> v; u--, v--;
			base_flower.add_edge(2+u, 2+N+v, N+1);
		}
		auto query_dir = [&](int l_weight, int r_weight) -> std::array<int, 2> {
			assert(l_weight <= N && r_weight <= N);
			Dinic flower = base_flower;
			for (int i = 0; i < N; i++) {
				flower.add_edge(st, 2+i, l_weight);
			}
			for (int i = 0; i < N; i++) {
				flower.add_edge(2+N+i, en, r_weight);
			}
			int flow = flower.max_flow(st, en);
			// find every guy that has level != -1
			assert(flower.level[st] != -1);
			assert(flower.level[en] == -1);
			std::array<int, 2> ans{0, 0};
			for (int i = 0; i < N; i++) {
				if (flower.level[2+i] == -1) ans[0]++;
			}
			for (int i = 0; i < N; i++) {
				if (flower.level[2+N+i] != -1) ans[1]++;
			}
			assert(flow == ans[0] * l_weight + ans[1] * r_weight);
			return ans;
		};

		std::vector<std::array<int, 2>> pareto; pareto.reserve(N);
		pareto.push_back({N, 0});
		[&](this auto&& self, std::array<int, 2> mi, std::array<int, 2> ma) -> void {
			assert(mi[0] >= ma[0]);
			assert(mi[1] <= ma[1]);
			int lw = ma[1] - mi[1];
			int rw = mi[0] - ma[0];
			assert(lw >= 0 && rw >= 0);
			int vmi = lw * mi[0] + rw * mi[1];
			int vma = lw * ma[0] + rw * ma[1];
			assert(vmi == vma);
			auto md = query_dir(lw, rw);
			int vmd = lw * md[0] + rw * md[1];
			assert(vmd <= vmi);
			if (vmd == vmi) {
				return;
			} else {
				self(mi, md);
				pareto.push_back(md);
				self(md, ma);
			}
		}({N, 0}, {0, N});
		pareto.push_back({0, N});

		for (auto& [a, b] : pareto) {
			a = N-a;
		}

		for (auto [a, b] : pareto) cerr << a << ' ' << b << '\n';
		cout << fixed << setprecision(20);
		for (int q = 0; q < Q; q++) {
			int64_t a, b, k; cin >> a >> b >> k;
			int mi = 0, ma = int(pareto.size());
			while (ma - mi > 1) {
				int md = (mi + ma) / 2;
				if (a * pareto[md][0] + b * pareto[md][1] <= k) mi = md;
				else ma = md;
			}
			if (ma == int(pareto.size())) {
				cout << a * pareto.back()[0] << '\n';
			} else {
				// mi .. ma
				int64_t vmi = a * pareto[mi][0] + b * pareto[mi][1];
				int64_t vma = a * pareto[ma][0] + b * pareto[ma][1];
				if (vmi == k) {
					cout << a * pareto[mi][0] << '\n';
				} else {
					int64_t diff = vma - vmi;
					int64_t tgt = k - vmi;
					cout << a * (pareto[mi][0] + double(pareto[ma][0] - pareto[mi][0]) * double(tgt) / double(diff)) << '\n';
				}
			}
		}
	}

	return 0;
}
