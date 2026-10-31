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
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N, M; cin >> N >> M;
	std::vector<std::pair<std::array<int, 2>, int>> edges(M);
	std::vector<std::vector<std::pair<int, int>>> adj(N);
	for (int e = 0; e < M; e++) {
		int a, b, c; cin >> a >> b >> c; a--, b--;
		edges[e] = {{a, b}, c};
		if (c) {
			adj[a].emplace_back(b, e);
			adj[b].emplace_back(a, e);
		}
	}

	std::vector<int> depth(N);
	std::vector<std::pair<int, int>> par_edge(N);
	depth[0] = 0, par_edge[0] = {-1, -1};
	std::y_combinator([&](auto self, int cur, int prv) -> void {
		for (auto [nxt, e] : adj[cur]) {
			if (nxt == prv) continue;
			depth[nxt] = depth[cur] + 1;
			par_edge[nxt] = {cur, e};
			self(nxt, cur);
		}
	})(0, -1);

	std::vector<int> par(N, -1);
	auto get_par = [&](int a) -> int {
		while (par[a] >= 0) {
			if (par[par[a]] >= 0) par[a] = par[par[a]];
			a = par[a];
		}
		return a;
	};

	int next_label = 1;
	std::vector<int> ans(M, -1);
	std::vector<int> edge_queue; edge_queue.reserve(M);

	for (int e = 0; e < M; e++) {
		if (ans[e] != -1) continue;
		auto [a, b] = edges[e].first;
		a = get_par(a), b = get_par(b);
		while (a != b) {
			if (depth[a] > depth[b]) std::swap(a, b);
			assert(par[b] < 0);
			auto [nb, ne] = par_edge[b];
			edge_queue.push_back(ne);
			assert(ans[ne] == -1);
			par[b] = nb;
			b = get_par(b);
		}
		std::sort(edge_queue.begin(), edge_queue.end());
		for (auto ne : edge_queue) {
			assert(ans[ne] == -1);
			ans[ne] = next_label++;
		}
		edge_queue.clear();

		if (ans[e] != -1) continue;
		ans[e] = next_label++;
	}

	for (int e = 0; e < M; e++) {
		cout << ans[e] << " \n"[e+1==M];
	}

	return 0;
}
