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
			cin >> vs[0] >> vs[1] >> w; vs[0]--, vs[1]--;
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
			//cerr << "edge " << w << ' ' << u << '-' << v << '\n';
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
		auto ans = [&](this auto&& self, int cur, int p_w) -> std::pair<int, std::priority_queue<int64_t>> {
			if (cur < N) {
				std::priority_queue<int64_t> deltas;
				deltas.push(p_w);
				return {need[cur], std::move(deltas)};
			} else {
				//cerr << cur << ' ' << weight[cur] << ' ' << ch[cur][0] << ',' << ch[cur][1] << '\n';
				int w = weight[cur];
				auto [lcnt, lval] = self(ch[cur][0], w);
				auto [rcnt, rval] = self(ch[cur][1], w);
				int cnt = lcnt + rcnt;
				if (lval.size() < rval.size()) {
					std::swap(lval, rval);
				}
				std::priority_queue<int64_t> res = std::move(lval);
				while (!rval.empty()) {
					res.push(rval.top());
					rval.pop();
				}
				assert(p_w >= w);
				int64_t ntop = int64_t(p_w - w) * cnt + res.top();
				res.pop();
				res.push(ntop);
				return {cnt, std::move(res)};
			}
		}(2*N-2, weight[2*N-2]);
		assert(ans.first == P);
		for (int i = 1; i <= N; i++) {
			cout << ans.second[i-1] << " \n"[i==N];
		}
	}

	return 0;
}
