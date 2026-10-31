#include <bits/stdc++.h>

template <typename T> void setmax(T& a, T b) {
	if (b > a) a = b;
}

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N, M; cin >> N >> M;
		cerr << '\n' << "start " << N << ' ' << M << '\n';
		struct edge_t {
			std::array<std::array<int, 2>, 2> v;
			edge_t rev() {
				return edge_t{{std::array<int, 2>{v[0][0], v[1][0]}, {v[0][1], v[1][1]}}};
			}
		};
		using vert_t = std::array<int, 2>;
		std::map<std::array<int, 2>, edge_t> mp;
		std::vector<std::set<int>> adj(N);
		std::vector<vert_t> vert_val(N, {0, 0});
		for (int e = 0; e < M; e++) {
			int u, v; cin >> u >> v;
			u--, v--;
			if (u > v) std::swap(u, v);
			assert(!mp.count({u, v}));
			mp.insert({{u, v}, edge_t{{std::array<int, 2>{0, 0}, {0, 1}}}});
			adj[u].insert(v);
			adj[v].insert(u);
		}
		std::vector<bool> in_queue(N, false);
		std::vector<int> q; q.reserve(N);
		auto check_q = [&](int i) -> void {
			if (!in_queue[i] && int(adj[i].size()) <= 2) {
				in_queue[i] = true;
				q.push_back(i);
			}
		};
		for (int i = 0; i < N; i++) {
			check_q(i);
		}

		auto merge_parallel = [&](edge_t a, edge_t b) -> edge_t {
			edge_t r{{std::array<int, 2>{0, 0}, {0, 0}}};
			for (int ai = 0; ai <= 1; ai++) {
				for (int bi = 0; ai + bi <= 1; bi++) {
					for (int aj = 0; aj <= 1; aj++) {
						for (int bj = 0; aj + bj <= 1; bj++) {
							setmax(r.v[ai+bi][aj+bj], a.v[ai][aj] + b.v[bi][bj]);
						}
					}
				}
			}
			cerr << "merge_parallel result " << r.v[0][0] << ' ' << r.v[0][1] << ' ' << r.v[1][0] << ' ' << r.v[1][1] << '\n';
			return r;
		};
		auto merge_series = [&](edge_t a, vert_t v, edge_t b) -> edge_t {
			edge_t r{{std::array<int, 2>{0, 0}, {0, 0}}};
			for (int ai = 0; ai <= 1; ai++) {
				for (int aj = 0; aj <= 1; aj++) {
					for (int bj = 0; aj + bj <= 1; bj++) {
						for (int bk = 0; bk <= 1; bk++) {
							setmax(r.v[ai][bk], a.v[ai][aj] + v[1-(aj+bj)] + b.v[bj][bk]);
						}
					}
				}
			}
			cerr << "merge_series result " << r.v[0][0] << ' ' << r.v[0][1] << ' ' << r.v[1][0] << ' ' << r.v[1][1] << '\n';
			return r;
		};
		auto merge_leaf = [&](vert_t v, edge_t e, vert_t l) -> vert_t {
			vert_t r{0, 0};
			for (int a = 0; a <= 1; a++) {
				for (int i = 0; a+i <= 1; i++) {
					for (int j = 0; j <= 1; j++) {
						setmax(r[a+i], v[a] + e.v[i][j] + l[1-j]);
					}
				}
			}
			cerr << "merge_leaf result " << r[0] << ' ' << r[1] << '\n';
			return r;
		};

		auto pop_edge = [&](int a, int b) -> edge_t {
			auto it = mp.find({std::min(a, b), std::max(a, b)});
			assert(it != mp.end());
			edge_t res = it->second;
			mp.erase(it);
			return a < b ? res : res.rev();
		};

		auto push_edge = [&](int a, int b, edge_t e) {
			assert(a < b);
			auto it = mp.find({a, b});
			if (it == mp.end()) {
				mp.insert({{a, b}, e});
				return;
			} else {
				cerr << "merge parallel " << a << ' ' << b << '\n';
				it->second = merge_parallel(it->second, e);
			}
		};

		for (int z = 0; z < int(q.size()); z++) {
			int cur = q[z];

			assert(adj[cur].size() <= 2);

			if (adj[cur].size() == 2) {
				int u = *adj[cur].begin();
				int v = *std::next(adj[cur].begin());
				adj[cur].clear();
				adj[u].erase(cur);
				adj[v].erase(cur);

				if (u > v) std::swap(u, v);
				assert(u < v);
				auto e1 = pop_edge(u, cur);
				auto e2 = pop_edge(cur, v);
				cerr << "merge series " << u << ' ' << cur << ' ' << v << '\n';
				auto e = merge_series(e1, vert_val[cur], e2);

				push_edge(u, v, e);

				adj[u].insert(v);
				adj[v].insert(u);

				check_q(u);
				check_q(v);
			} else if (adj[cur].size() == 1) {
				int o = *adj[cur].begin();
				adj[o].erase(cur);
				adj[cur].clear();

				auto e = pop_edge(o, cur);
				cerr << "merge leaf " << o << ' ' << cur << '\n';
				vert_val[o] = merge_leaf(vert_val[o], e, vert_val[cur]);

				check_q(o);
			} else if (adj[cur].size() == 0) {
				assert(z == N-1);
				cout << vert_val[cur][1] << '\n';
			} else assert(false);
		}
	}

	return 0;
}

// A K4 minor necessarily includes 6 simple cycles through a vertex, so this graph has no K4 minor.
// Thus, this graph is series-parallel.
