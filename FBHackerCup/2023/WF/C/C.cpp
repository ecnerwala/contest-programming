#include<bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	for (int case_num = 1; case_num <= T; case_num ++) {

		int N, M; cin >> N >> M;
		cerr << "Case #" << case_num << ": " << "N = " << N << "M = " << M << '\n';

		std::vector<int64_t> P(N);
		for (auto& p : P) cin >> p;
		std::vector<std::pair<int64_t, int>> vals(N);
		for (int i = 0; i < N; i++) {
			vals[i] = {P[i], i};
		}
		std::sort(vals.begin(), vals.end());
		std::vector<int> order(N);
		for (int i = 0; i < N; i++) {
			order[vals[i].second] = i;
		}
		std::vector<std::vector<int>> adj(N);
		for (int e = 0; e < M; e++) {
			int u, v; cin >> u >> v; u--, v--;
			assert(u != v);
			if (order[u] > order[v]) std::swap(u, v);
			adj[v].push_back(u);
		}

		struct par_t {
			int dest;
			int64_t min_update = 0;
		};
		std::vector<par_t> par(N, {-1, 0});
		auto merge = [&](par_t a, par_t b) -> par_t {
			return {b.dest, std::max(a.min_update, b.min_update)};
		};
		auto get_par = [&](int a) -> par_t {
			par_t res{a, 0};
			while (par[res.dest].dest >= 0) {
				if (par[par[res.dest].dest].dest >= 0) {
					par[res.dest] = merge(par[res.dest], par[par[res.dest].dest]);
				}
				res = merge(res, par[res.dest]);
			}
			return res;
		};
		for (auto [p, i] : vals) {
			for (int j : adj[i]) {
				auto a = get_par(i).dest;
				auto b = get_par(j).dest;
				if (a == b) continue;
				par[b] = {a, std::max(p - P[b], int64_t(0))};
				P[a] += P[b];
			}
		}

		int64_t ans = 0;
		for (int i = 0; i < N; i++) {
			ans += get_par(i).min_update;
		}
		cout << "Case #" << case_num << ": " << ans << '\n';
	}

	return 0;
}
