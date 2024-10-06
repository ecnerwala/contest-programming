#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N, M, P; cin >> N >> M >> P;
		std::vector<bool> need(N);
		for (int z = 0; z < P; z++) {
			int cur; cin >> cur; cur--;
			assert(!need[cur]);
			need[cur] = true;
		}
		std::vector<std::pair<int, std::array<int, 2>>> edges(M);
		for (auto& [w, vs] : edges) {
			cin >> w >> vs[0] >> vs[1]; vs[0]--, vs[1]--;
		}
		std::sort(edges.begin(), edges.end());
		std::vector<int> par(2*N-1, -1);
		std::vector<int> rep(2*N-1);
		auto get_par = [&](int a) -> int {
			while (par[a] >= 0) {
				if (par[par[a]] >= 0) {
					par[a] = par[par[a]];
				}
				a = par[a];
			}
			return a;
		};
		auto merge = [&](int a, int b) -> void {
			a = get_par(a), b = get_par(b);
			assert(a != b);
			if (par[a] > par[b]) std::swap(a, b);
			par[a] += par[b];
			par[b] = a;
		};
		int nxt_vert = N;
		std::vector<std::array<int, 2>> ch(2*N-1, {-1, -1});
		std::vector<int64_t> val(2*N-1);
		for (auto [w, vs] : edges) {
			auto [u, v] = vs;
			u = get_par(u), v = get_par(v);
			if (u == v) continue;
			int a = nxt_vert++;
			val[a] = w;
			ch[a] = {rep[u], rep[v]};

			merge(u, a);
			merge(v, a);
			rep[get_par(a)] = a;
		}
		assert(nxt_vert == 2*N-1);
		[&](this auto&& self, int cur) -> std::pair<int, std::vector<int64_t>> {
		}(2*N-2);
	}

	return 0;
}
