#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N, M, K; cin >> N >> M >> K;
	std::vector<int> C(N); for (int& c : C) { cin >> c; c--; }

	std::vector<int> par(2*N, -1);
	std::vector<std::pair<int, int>> ops; ops.reserve(2*N);
	auto get_par = [&](int a) -> int {
		while (par[a] >= 0) a = par[a];
		return a;
	};
	auto merge = [&](int a, int b) -> bool {
		a = get_par(a);
		b = get_par(b);
		if (a == b) return false;
		if (par[a] > par[b]) std::swap(a, b);
		ops.push_back({b, par[b]});
		par[a] += par[b];
		par[b] = a;
		return true;
	};
	auto rollback_all = [&]() -> void {
		while (!ops.empty()) {
			auto [b, s] = ops.back();
			ops.pop_back();
			int a = par[b];
			par[a] -= s;
			par[b] = s;
		}
	};

	// This lets us mark which components are always bad internally
	std::vector<bool> is_bad(K);

	std::map<std::array<int, 2>, std::vector<std::array<int, 2>>> cross_edges;
	for (int e = 0; e < M; e++) {
		int u, v; cin >> u >> v; u--, v--;
		if (C[u] == C[v]) {
			merge(2*u, 2*v+1);
			merge(2*u+1, 2*v);
			if (get_par(2*u) == get_par(2*u+1)) {
				is_bad[C[u]] = true;
			}
		} else {
			if (C[u] > C[v]) std::swap(u, v);
			cross_edges[{C[u], C[v]}].push_back({u, v});
		}
	}

	int num_good = 0;
	for (int k = 0; k < K; k++) {
		num_good += !is_bad[k];
	}

	int64_t ans = int64_t(num_good) * int64_t(num_good - 1) / 2;

	ops.clear();

	for (auto [comps, edges] : cross_edges) {
		if (is_bad[comps[0]] || is_bad[comps[1]]) continue;
		for (auto [u, v] : edges) {
			merge(2*u, 2*v+1);
			merge(2*u+1, 2*v);
			if (get_par(2*u) == get_par(2*u+1)) {
				ans--;
				break;
			}
		}
		rollback_all();
	}

	cout << ans << '\n';

	return 0;
}
