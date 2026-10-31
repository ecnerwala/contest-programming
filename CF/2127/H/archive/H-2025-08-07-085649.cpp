#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N, M; cin >> N >> M;
		struct edge_t {
			std::array<std::array<int, 2>, 2> v;
		};
		std::map<std::array<int, 2>, edge_t> mp;
		std::vector<std::set<int>> adj(N);
		std::vector<std::array<int, 2>> vert_val(N, {0, 0});
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

		for (int z = 0; z < int(q.size()); z++) {
			int cur = q[z];

			cerr << cur << ' ' << adj[cur].size() << '\n';

			assert(adj[cur].size() <= 2);

			if (adj[cur].size() == 2) {
				int u = *adj[cur].begin();
				int v = *std::next(adj[cur].begin());
				adj[cur].clear();
				adj[u].erase(cur);
				adj[v].erase(cur);

				adj[u].insert(v);
				adj[v].insert(u);

				check_q(u);
				check_q(v);
			} else if (adj[cur].size() == 1) {
				int o = *adj[cur].begin();
				adj[o].erase(cur);
				adj[cur].clear();

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
