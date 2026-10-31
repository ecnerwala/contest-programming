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
	int V = N*M;
	auto vert = [&](int i, int j) { assert(0 <= i && i < N && 0 <= j && j < M); return i * M + j; };

	vector<int> color(V);
	for (int i = 0; i < N; i++) {
		string S; cin >> S;
		for (int j = 0; j < M; j++) {
			color[vert(i, j)] = S[j] - '0';
			assert(color[vert(i, j)] == 0 || color[vert(i, j)] == 1);
		}
	}

	vector<int> par(V, -1);
	auto get_par = [&](int a) -> int {
		while (par[a] >= 0) {
			if (par[par[a]] >= 0) par[a] = par[par[a]];
			a = par[a];
		}
		return a;
	};
	auto merge = [&](int a, int b) -> bool {
		a = get_par(a), b = get_par(b);
		if (a == b) return false;
		if (par[a] > par[b]) std::swap(a, b);
		par[a] += par[b];
		par[b] = a;
		return true;
	};
	vector<vector<int>> adj(V);
	for (int z = 0; z < 2; z++) {
		auto check_edge = [&](int a, int b) -> void {
			if ((color[a] ^ color[b]) != z) return;
			if (merge(a, b)) {
				adj[a].push_back(b);
				adj[b].push_back(a);
			}
		};
		for (int i = 0; i < N; i++) {
			for (int j = 0; j+1 < M; j++) {
				check_edge(vert(i, j), vert(i, j+1));
			}
		}
		for (int i = 0; i+1 < N; i++) {
			for (int j = 0; j < M; j++) {
				check_edge(vert(i, j), vert(i+1, j));
			}
		}
		for (int i = 0; i+1 < N; i += 2) {
			for (int j = 0; j+1 < M; j++) {
				check_edge(vert(i, j), vert(i+1, j+1));
			}
		}
		for (int i = 1; i+1 < N; i += 2) {
			for (int j = 0; j+1 < M; j++) {
				check_edge(vert(i, j+1), vert(i+1, j));
			}
		}
	}

	int max_depth = 0;
	int max_mid = 0;
	int max_diam = 0;
	std::y_combinator([&](auto self, int cur, int prv, int d) -> void {
		max_depth = max(max_depth, d);
		max_mid = max(max_mid, max_depth - 2 * d);
		max_diam = max(max_diam, max_mid + d);
		if (color[cur] == 0) {
			// we have an extra vertex
			max_depth = max(max_depth, d+1);
			max_mid = max(max_mid, max_depth - 2 * (d+1));
			max_diam = max(max_diam, max_mid + (d+1));

			max_depth = max(max_depth, d);
			max_mid = max(max_mid, max_depth - 2 * d);
			max_diam = max(max_diam, max_mid + d);
		}
		for (int nxt : adj[cur]) {
			if (nxt == prv) continue;
			self(nxt, cur, d + (color[cur] ^ color[nxt]));
			max_depth = max(max_depth, d);
			max_mid = max(max_mid, max_depth - 2 * d);
			max_diam = max(max_diam, max_mid + d);
		}
	})(0, -1, 0);

	cout << (max_diam + 1) / 2 << '\n';

	return 0;
}
