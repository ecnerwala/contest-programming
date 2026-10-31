#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N, M; cin >> N >> M;
		struct edge_t {
			std::array<int, 2> vs;
			int w;
		};
		std::vector<edge_t> edges(M);
		for (auto& e : edges) {
			cin >> e.vs[0] >> e.vs[1] >> e.w;
			e.vs[0]--, e.vs[1]--;
		}
		cout << [&]() -> int64_t {
			std::sort(edges.begin(), edges.end(), [&](auto a, auto b) { return a.w < b.w; });
			int64_t base_val = 0;
			for (int e = 0; e < N-1; e++) {
				base_val += edges[e].w;
			}
			std::vector<int> par(N, -1);
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
			for (int e = 0; e < N-1; e++) {
				auto [u, v] = edges[e].vs;
				if (!merge(u, v)) {
					return base_val;
				}
			}
			if (M == N-1) return -1;
			assert(M >= N);
			if (M == N) {
				std::vector<int> deg(N);
				for (auto [vs, w] : edges) {
					deg[vs[0]]++;
					deg[vs[1]]++;
				}
				bool is_cyc = true;
				for (int i = 0; i < N; i++) {
					if (deg[i] == 1) is_cyc = false;
				}
				if (is_cyc) return -1;
			}

			if (edges.back().w <= 2) {
				if (edges[N-1].w == 1) {
					std::vector<int> deg_1(N);
					for (auto [vs, w] : edges) {
						if (w == 2) break;
						deg_1[vs[0]]++;
						deg_1[vs[1]]++;
					}
					for (int i = 0; i < N; i++) {
						if (deg_1[i] != 2) return base_val;
					}
					return base_val + 1;
				}
				std::fill(par.begin(), par.end(), -1);
				int num_ccs = N;
				std::vector<int> deg_1(N);
				for (auto [vs, w] : edges) {
					if (w == 2) break;
					auto did_merge = merge(vs[0], vs[1]);
					assert(did_merge);
					num_ccs--;
					deg_1[vs[0]]++;
					deg_1[vs[1]]++;
				}
				assert(base_val == (N-1) + num_ccs - 1);
				assert(num_ccs >= 1);
				if (num_ccs == 1) {
					std::vector<int> ends;
					for (int i = 0; i < N; i++) {
						if (deg_1[i] == 1) ends.push_back(i);
					}
					if (int(ends.size()) == 2) {
						for (auto [vs, w] : edges) {
							if (w != 2) continue;
							if (vs[0] == ends[0] && vs[1] == ends[1]) {
							} else if (vs[1] == ends[0] && vs[0] == ends[1]) {
							} else {
								return (N-1) + num_ccs;
							}
						}
						return (N-1) + num_ccs + 1;
					}
					return (N-1) + num_ccs;
				}
				assert(num_ccs >= 2);
				if (num_ccs == 2) {
					for (auto [vs, w] : edges) {
						if (w != 2) continue;
						if (get_par(vs[0]) == get_par(vs[1])) {
							return (N-1) + num_ccs - 1;
						}
					}
					// all parallel is no good
					return (N-1) + num_ccs;
				} else {
					if (M > N) return (N-1) + num_ccs - 1;
					else {
						std::vector<int> deg(N);
						// check if they're all 1 big cycle
						for (auto [vs, w] : edges) {
							if (w != 2) continue;
							deg[get_par(vs[0])]++;
							deg[get_par(vs[1])]++;
						}
						for (auto d : deg) if (d == 1) return base_val;
						return base_val + 1;
					}
				}
			}
			assert(false);
		}() << '\n';
	}

	return 0;
}
