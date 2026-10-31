#include<bits/stdc++.h>

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

struct seg_tree {
	struct seg_node {
		int best_path;
		int mid_len;
		int mid_parity;
		int best_down_path[2];
		int best_up_path[2];
	};
	int N;
	std::vector<seg_node> seg;
	seg_tree(int N_) : N(N_), seg(2*N) {
	}
};

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);
	int N; cin >> N;
	struct edge_t {
		array<int, 2> v;
		int t;
	};
	vector<edge_t> edges(N-1);
	for (auto& e : edges) {
		int u, v, t; cin >> u >> v >> t; u--, v--;
		e.v = {u,v};
		e.t = t;
	}

	vector<vector<int>> adj(N);
	for (int e = 0; e < N-1; e++) {
		adj[edges[e].v[0]].push_back(e);
		adj[edges[e].v[1]].push_back(e);
	}

	vector<int> sz(N);
	// Build the sizes
	std::y_combinator([&](auto self, int cur, int prvE) -> void {
		sz[cur]++;
		for (int e : adj[cur]) {
			if (e == prvE) continue;
			if (edges[e].v[1] == cur) {
				swap(edges[e].v[0], edges[e].v[1]);
			}
			assert(edges[e].v[0] == cur);
			int nxt = edges[e].v[1];
			self(nxt, e);
			sz[cur] += sz[nxt];
		}
	})(0, -1);

	// Build the HLD
	std::y_combinator([&](auto self, int cur) -> void {
	});

	int Q; cin >> Q;
	while (Q--) {
		int e; cin >> e; e--;
		// TODO: flip and then 
	}

	return 0;
}
