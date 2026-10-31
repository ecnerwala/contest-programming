#include <bits/stdc++.h>

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
	std::ios_base::sync_with_stdio(false); std::cin.tie(nullptr);

	int N; cin >> N;
	std::vector<std::vector<int>> adj(N);
	std::vector<int> edge_ends(2*N-2);
	for (int e = 0; e < N-1; e++) {
		int u, v; cin >> u >> v; u--, v--;
		edge_ends[2*e] = u;
		adj[u].push_back(2*e);
		edge_ends[2*e+1] = v;
		adj[v].push_back(2*e+1);
	}

	int64_t ans = 0;

	std::vector<int> ant_par(N, -1);
	auto get_ant_par = [&](int a) -> int {
		while (ant_par[a] >= 0) {
			if (ant_par[ant_par[a]] >= 0) ant_par[a] = ant_par[ant_par[a]];
			a = ant_par[a];
		}
		return a;
	};
	auto merge_ants = [&](int a, int b) -> int {
		assert(ant_par[a] < 0 && ant_par[b] < 0);
		assert(a != b);
		if (ant_par[a] > ant_par[b]) std::swap(a, b);
		ans += int64_t(-ant_par[a]) * int64_t(-ant_par[b]);
		ant_par[a] += ant_par[b];
		ant_par[b] = a;
		return a;
	};

	std::vector<int> edge_par(N-1, -1);
	auto get_edge_par = [&](int a) -> int {
		while (edge_par[a] >= 0) {
			if (edge_par[edge_par[a]] >= 0) edge_par[a] = edge_par[edge_par[a]];
			a = edge_par[a];
		}
		return a;
	};

	std::vector<std::array<int, 2>> neighbor(3*N-2, {-1, -1});
	for (int e = 0; e < N-1; e++) {
		neighbor[2*e][0] = 2*e+1;
		neighbor[2*e+1][0] = 2*e;
	}

	std::vector<int> ant_loc(N, -1);

	auto dfs = std::y_combinator([&](auto self, int prvE) -> void {
		int cur = edge_ends[prvE];
		int L = 0;
		int cur_ch = -1;
		int last_edge = -1;
		for (int e : adj[cur]) {
			if (e == prvE) continue;
			self(e^1);
			if (neighbor[e][0] != (e^1)) {
			}
			if (cur_ch == -1) cur_ch = nxt;
			else cur_ch = merge_ants(cur_ch, nxt);
			last_edge = e^1;
		}
		if (adj[cur].size() == 2) {
		}
	});

	int Q; cin >> Q;
	while (Q--) {
		int ant_id; cin >> ant_id; ant_id--;
		ant_id = get_ant_par(ant_id);

		if (ant_loc[ant_id] == -1) {
			// this ant is its own vertex
		} else {
			int e = ant_loc[ant_id];
			e = get_edge_par(e);
		}
	}
}
