#include <bits/stdc++.h>

template <typename T> void setmin(T& a, T b) { if (b < a) a = b; }

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		const int64_t INF = 1e18;
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
		std::vector<int> rep(2*N-1, -1); std::iota(rep.begin(), rep.begin() + N, 0);
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
		std::vector<int> weight(2*N-1);
		for (auto [w, vs] : edges) {
			auto [u, v] = vs;
			cerr << "edge " << w << ' ' << u << '-' << v << '\n';
			u = get_par(u), v = get_par(v);
			if (u == v) continue;
			int a = nxt_vert++;
			weight[a] = w;
			ch[a] = {rep[u], rep[v]};

			merge(u, a);
			merge(v, a);
			rep[get_par(a)] = a;
		}
		assert(nxt_vert == 2*N-1);
		auto ans = [&](this auto&& self, int cur) -> std::pair<int, std::vector<int64_t>> {
			if (cur < N) {
				return {need[cur], std::vector<int64_t>(1, 0)};
			} else {
				cerr << cur << ' ' << weight[cur] << ' ' << ch[cur][0] << ',' << ch[cur][1] << '\n';
				auto [lcnt, lval] = self(ch[cur][0]);
				auto [rcnt, rval] = self(ch[cur][1]);
				int64_t w = weight[cur];
				std::vector<int64_t> res(int(lval.size()) + int(rval.size()), INF);
				for (int i = 0; i <= int(lval.size()); i++) {
					for (int j = 0; j <= int(rval.size()); j++) {
						if (i + j == 0) continue;
						int64_t lv = i ? lval[i-1] : int64_t(w) * lcnt; 
						int64_t rv = j ? rval[j-1] : int64_t(w) * rcnt;
						setmin(res[i+j-1], lv + rv);
					}
				}
				return {lcnt + rcnt, res};
			}
		}(2*N-2);
		assert(ans.first == P);
		for (int i = 1; i <= N; i++) {
			cout << ans.second[i-1] << " \n"[i==N];
		}
	}

	return 0;
}
