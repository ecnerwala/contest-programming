#include<bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	for (int case_num = 1; case_num <= T; case_num ++) {
		int N, M; cin >> N >> M;
		std::vector<int> label(N);
		for (auto& v : label) cin >> v;
		std::vector<std::vector<int>> adj_orig(N);
		for (int e = 0; e < M; e++) {
			int u, v; cin >> u >> v; u--, v--;
			adj_orig[u].push_back(v);
			adj_orig[v].push_back(u);
		}

		cerr << "Case #" << case_num << " " << N << ' ' << M << '\n';

		std::vector<std::vector<int>> cycs; cycs.reserve(N);
		{
			std::vector<int> par(N, -1);
			std::vector<int> depth(N, -1);
			std::vector<int> lowval(N, -1);
			std::vector<int> stk; stk.reserve(N+1);
			[&](this auto&& self, int cur, int prv) -> void {
				par[cur] = prv;
				depth[cur] = prv == -1 ? 0 : depth[prv] + 1;
				lowval[cur] = depth[cur];
				for (int nxt : adj_orig[cur]) {
					if (depth[nxt] == -1) {
						int stk_sz = int(stk.size());
						stk.push_back(nxt);
						self(nxt, cur);
						if (lowval[nxt] >= depth[cur]) {
							stk.push_back(cur);
							cycs.emplace_back(stk.begin() + stk_sz, stk.end());
							stk.resize(stk_sz);
						} else {
							lowval[cur] = std::min(lowval[cur], lowval[nxt]);
						}
					} else if (depth[nxt] < depth[cur]) {
						lowval[cur] = std::min(lowval[cur], depth[nxt]);
					}
				}
			}(0, -1);
			assert(stk.empty());
		}
		int C = int(cycs.size());
		std::vector<std::vector<std::pair<int, int>>> cyc_adj(N);
		for (int c = 0; c < C; c++) {
			for (int i = 0; i < int(cycs[c].size()); i++) {
				cyc_adj[cycs[c][i]].push_back({c, i});
			}
		}
		cerr << "C = " << C << '\n';
		//for (auto c : cycs) { for (auto v : c) { cerr << v << ' '; } cerr << '\n'; }
		{
			std::vector<int> lens(N+1);
			for (auto c : cycs) {
				lens[int(c.size())]++;
			}
			for (int i = 0; i <= N; i++) if (lens[i]) cerr << " len " << i << " x " << lens[i] << '\n';
		}

		int64_t ans = 0;

		using bs = bitset<121>;
		auto solve = [&](std::vector<std::array<bs, 2>> vals) {
			//for (auto v : vals) { cerr << v[0] << ' ' << v[1] << '\n'; }
			bs deads;
			std::vector<int> par(N+1, -1);
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
				if (a > b) std::swap(a, b);
				assert(a < b);
				deads[a] = true;
				par[a] = b;
				return true;
			};
			while (true) {
				std::vector<std::array<bs, 2>> nvals; nvals.reserve(vals.size());
				for (auto v : vals) {
					// if one is a subset of another
					if ((v[0] | v[1]) == v[0] || (v[0] | v[1]) == v[1]) {
						bs to_fix = v[0] | v[1];
						for (int i = int(to_fix._Find_first()); i < int(to_fix.size()); i = int(to_fix._Find_next(i))) {
							merge(i, N);
						}
						// we don't need this pair anymore
						continue;
					}
					assert(v[0].any());
					assert(v[1].any());
					if (v[0].count() == 1 && v[1].count() == 1) {
						int a = int(v[0]._Find_first());
						int b = int(v[1]._Find_first());
						// these guys can be substitutes
						merge(a, b);
						// we don't need this pair anymore
						continue;
					}
					{
						// if there's anything shared, delete all of them
						bs to_fix = v[0] & v[1];
						for (int i = int(to_fix._Find_first()); i < int(to_fix.size()); i = int(to_fix._Find_next(i))) {
							merge(i, N);
						}
					}
					nvals.push_back({v[0], v[1]});
				}
				for (auto& v : nvals) {
					for (auto& x : v) {
						bs to_fix = x & deads;
						for (int i = int(to_fix._Find_first()); i < int(to_fix.size()); i = int(to_fix._Find_next(i))) {
							x[i] = false;
							int p = get_par(i);
							if (p < N) x[p] = true;
						}
						x[N] = false;
					}
				}
				if (vals == nvals) break;
				vals = nvals;
			}
			for (auto v : vals) {
				assert(v[0].count() > 0);
				assert(v[1].count() > 0);
				assert(v[0].count() + v[1].count() >= 3);
			}
			int mex = 0;
			if (int(vals.size()) >= 20) cerr << "brute " << int(vals.size()) << '\n';
			[&](this auto&& self, int idx, bs v) -> void {
				if (idx == int(vals.size())) {
					mex = std::max(mex, int((~v)._Find_first()));
					return;
				} else {
					self(idx+1, v | vals[idx][0]);
					self(idx+1, v | vals[idx][1]);
				}
			}(0, deads);
			ans += mex;
		};

		for (int st = 0; st < N; st++) {
			std::vector<std::array<bs, 2>> vals; vals.reserve(C+1);
			bs v0; v0[label[st]] = true;
			vals.push_back({v0, v0});
			[&](this auto&& self, int cur, int prv_c) -> void {
				if (cur > st) {
					// find the mex of the path
					//cerr << "solve " << st << ' ' << cur << '\n';
					solve(vals);
				}
				for (auto [c, cur_i] : cyc_adj[cur]) {
					if (c == prv_c) continue;
					std::vector<int> cyc = cycs[c];
					assert(0 <= cur_i && cur_i < int(cyc.size()));
					std::rotate(cyc.begin(), cyc.begin() + cur_i, cyc.end());
					assert(cyc[0] == cur);
					int L = int(cyc.size());
					std::vector<bs> pref(L);
					std::vector<bs> suff(L);
					pref[0][label[cyc[0]]] = true;
					for (int i = 1; i < L; i++) {
						pref[i] = pref[i-1];
						pref[i][label[cyc[i]]] = true;
					}
					suff[L-1][label[cyc[0]]] = true;
					for (int i = L-1; i > 0; i--) {
						suff[i][label[cyc[i]]] = true;
						suff[i-1] = suff[i];
					}
					for (int i = 1; i < L; i++) {
						vals.push_back({pref[i], suff[i]});
						self(cyc[i], c);
						vals.pop_back();
					}
				}
			}(st, -1);
		}

		cout << "Case #" << case_num << ": " << ans << '\n' << flush;
	}

	return 0;
}
