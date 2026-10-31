#include<bits/stdc++.h>

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

struct seg {
	std::vector<int64_t> v;
	seg() : v() {}
	seg(int N) : v(N+1, 0) {}
	void init() {
		for (int i = 1; i < int(v.size()); i++) v[i] += v[i-1];
	}
	void set_val(int a, int64_t val) {
		v[a+1] = val;
	}
	int64_t query(int l, int r) {
		assert(l <= r);
		return v[r] - v[l];
	}
};

template <typename T> void setmin(T& a, const T& b) { if (b < a) a = b; }

int main() {
	using namespace std;

	ios_base::sync_with_stdio(false), cin.tie(nullptr), cout.tie(nullptr);
	int T; cin >> T;

	for (int case_num = 1; case_num <= T; case_num ++) {
		using i64 = int64_t;
		int K, Q; cin >> K >> Q;
		string A; cin >> A;
		A = "(" + A + ")";
		struct op_cost {
			i64 L = 0, R = 0, P = 0;
		};
		vector<op_cost> cost(K+2);
		for (int i = 1; i <= K; i++) cin >> cost[i].L;
		for (int i = 1; i <= K; i++) cin >> cost[i].R;
		for (int i = 1; i <= K; i++) cin >> cost[i].P;

		vector<pair<int, int>> queries(Q);
		for (int q = 0; q < Q; q++) cin >> queries[q].first;
		for (int q = 0; q < Q; q++) cin >> queries[q].second;

		// TODO: don't overflow plz
		cost[1].L = 1e18;
		cost[K].R = 1e18;

		const int V = K/2 + 1;
		vector<pair<int, bool>> paren_node(K+2);
		vector<array<int, 2>> node_inds; node_inds.reserve(V);
		vector<vector<int>> ch(V);
		vector<array<int, 20>> par(V);
		vector<int> parInd(V);
		vector<int> depth(V);
		{
			node_inds.push_back({0, K+1});
			fill(par[0].begin(), par[0].end(), 0);
			parInd[0] = -1;
			depth[0] = 0;
			vector<int> stk; stk.reserve(V); stk.push_back(0);
			for (int i = 1; i <= K; i++) {
				if (A[i] == '(') {
					int ind = int(node_inds.size());
					paren_node[i] = {ind, 0};
					node_inds.push_back({i, -1});

					fill(par[ind].begin(), par[ind].end(), 0);
					par[ind][0] = stk.back();
					for (int z = 0; par[ind][z]; z++) {
						par[ind][z+1] = par[par[ind][z]][z];
					}
					depth[ind] = depth[par[ind][0]]+1;
					parInd[ind] = int(ch[par[ind][0]].size());
					ch[par[ind][0]].push_back(ind);

					stk.push_back(ind);
				} else if (A[i] == ')') {
					int ind = stk.back();

					paren_node[i] = {ind, 1};
					node_inds[ind][1] = i;

					stk.pop_back();
				}
			}
		}

		// child side, parent side, direction (0 is from child -> parent)
		using par_dist_t = array<array<array<i64, 2>, 2>, 2>;
		vector<par_dist_t> parDist(V);
		vector<array<i64, 2>> swapDist(V); // from z to !z
		vector<array<seg, 2>> chSegs(V);
		std::y_combinator([&](auto self, int cur) -> void {
			for (int nxt : ch[cur]) {
				self(nxt);
			}

			int a = node_inds[cur][0], b = node_inds[cur][1];
			swapDist[cur] = {cost[a].P, cost[b].P};

			int C = int(ch[cur].size());
			chSegs[cur][0] = seg(C * 2 + 1);
			chSegs[cur][1] = seg(C * 2 + 1);
			chSegs[cur][0].set_val(0, cost[a].R);
			chSegs[cur][1].set_val(2 * C, cost[b].L);
			for (int z = 0; z < int(ch[cur].size()); z++) {
				int nxt = ch[cur][z];
				chSegs[cur][1].set_val(2 * z + 0, cost[node_inds[nxt][0]].L);
				chSegs[cur][0].set_val(2 * z + 1, swapDist[nxt][0]);
				chSegs[cur][1].set_val(2 * z + 1, swapDist[nxt][1]);
				chSegs[cur][0].set_val(2 * z + 2, cost[node_inds[nxt][1]].R);
			}
			chSegs[cur][0].init();
			chSegs[cur][1].init();

			setmin(swapDist[cur][0], chSegs[cur][0].query(0, 2 * C + 1));
			setmin(swapDist[cur][1], chSegs[cur][1].query(0, 2 * C + 1));

			for (int z = 0; z < int(ch[cur].size()); z++) {
				int nxt = ch[cur][z];
				// dir = 0 -> child->parent
				parDist[nxt][0][0][0] = chSegs[cur][1].query(0, 2 * z + 1);
				parDist[nxt][1][0][0] = chSegs[cur][1].query(0, 2 * z + 2);
				parDist[nxt][0][1][0] = chSegs[cur][0].query(2 * z + 1, 2 * C + 1);
				parDist[nxt][1][1][0] = chSegs[cur][0].query(2 * z + 2, 2 * C + 1);

				// dir = 1 -> parent->child
				parDist[nxt][0][0][1] = chSegs[cur][0].query(0, 2 * z + 1);
				parDist[nxt][1][0][1] = chSegs[cur][0].query(0, 2 * z + 2);
				parDist[nxt][0][1][1] = chSegs[cur][1].query(2 * z + 1, 2 * C + 1);
				parDist[nxt][1][1][1] = chSegs[cur][1].query(2 * z + 2, 2 * C + 1);
			}
		})(0);

		auto merge = [&](const par_dist_t& pa, const par_dist_t& gp) -> par_dist_t {
			par_dist_t res;
			for (int chd = 0; chd < 2; chd++) {
				for (int gpd = 0; gpd < 2; gpd++) {
					for (int dir = 0; dir < 2; dir++) {
						res[chd][gpd][dir] = min(pa[chd][0][dir] + gp[0][gpd][dir], pa[chd][1][dir] + gp[1][gpd][dir]);
					}
				}
			}
			return res;
		};

		vector<array<par_dist_t, 20>> parDistJump(V);
		std::y_combinator([&](auto self, int cur) -> void {
			parDistJump[cur][0] = parDist[cur];
			for (int z = 0; par[cur][z]; z++) {
				parDistJump[cur][z+1] = merge(parDistJump[cur][z], parDistJump[par[cur][z]][z]);
			}
			for (int nxt : ch[cur]) {
				setmin(swapDist[nxt][0], parDist[nxt][0][0][0] + swapDist[cur][0] + parDist[nxt][1][1][1]);
				setmin(swapDist[nxt][1], parDist[nxt][1][1][0] + swapDist[cur][1] + parDist[nxt][0][0][1]);

				for (int cs = 0; cs < 2; cs++) {
					for (int ps = 0; ps < 2; ps++) {
						for (int dir = 0; dir < 2; dir++) {
							// dir = 1 -> parent->child -> 0->1->cs
							setmin(parDist[nxt][cs][!ps][dir], parDist[nxt][cs][ps][dir] + swapDist[cur][dir ? !ps : ps]);
						}
					}
				}

				self(nxt);
			}
		})(0);

		auto get_anc = [&](int n, int k) -> int {
			assert(k >= 0);
			for (int z = 0; (1<<z) <= k; z++) {
				if (k & (1 << z)) {
					n = par[n][z];
				}
			}
			return n;
		};
		auto lca = [&](int a, int b) -> int {
			if (depth[a] > depth[b]) swap(a, b);
			b = get_anc(b, depth[b] - depth[a]);
			if (a == b) return a;
			int z = 0;
			while (par[a][z] != par[b][z]) z++;
			while (z) {
				--z;
				if (par[a][z] != par[b][z]) {
					a = par[a][z];
					b = par[b][z];
				}
			}
			return par[a][0];
		};
		auto get_par_dist = [&](int n, int k) -> par_dist_t {
			assert(k >= 0);
			par_dist_t ans;
			ans[0][0][0] = ans[0][0][1] = ans[1][1][0] = ans[1][1][1] = 0;
			ans[0][1][0] = swapDist[n][0];
			ans[0][1][1] = swapDist[n][1];
			ans[1][0][0] = swapDist[n][1];
			ans[1][0][1] = swapDist[n][0];
			for (int z = 0; (1<<z) <= k; z++) {
				if (k & (1 << z)) {
					ans = merge(ans, parDistJump[n][z]);
					n = par[n][z];
				}
			}
			return ans;
		};

		auto solve = [&](int s, int e) -> i64 {
			int sn = paren_node[s].first;
			int sd = paren_node[s].second;
			int en = paren_node[e].first;
			int ed = paren_node[e].second;

			int top = lca(sn, en);
			//cerr << sn << ' ' << sd << ' ' << en << ' ' << ed << ' ' << top << '\n';
			auto sp = get_par_dist(sn, depth[sn] - depth[top]);
			auto ep = get_par_dist(en, depth[en] - depth[top]);
			i64 best = min(sp[sd][0][0] + ep[ed][0][1], sp[sd][1][0] + ep[ed][1][1]);
			if (sn != top && en != top) {
				i64 cnd = 0;
				int sd2 = sn < en;
				int ed2 = en < sn;

				cnd += get_par_dist(sn, depth[sn] - depth[top] - 1)[sd][sd2][0];
				cnd += get_par_dist(en, depth[en] - depth[top] - 1)[ed][ed2][1];

				int si = 2 * parInd[get_anc(sn, depth[sn] - depth[top] - 1)] + 1 + sd2;
				int ei = 2 * parInd[get_anc(en, depth[en] - depth[top] - 1)] + 1 + ed2;

				if (sn < en) {
					cnd += chSegs[top][0].query(si, ei);
				} else {
					cnd += chSegs[top][1].query(ei, si);
				}
				best = min(best, cnd);
			}
			return best;
		};

#ifdef ECNERWALA
		cout << "Case #" << case_num << ":" << '\n';
		for (int q = 0; q < Q; q++) {
			i64 res = solve(queries[q].first, queries[q].second);
			cout << res << '\n';
		}
#else
		i64 ans = 0;
		for (int q = 0; q < Q; q++) {
			i64 res = solve(queries[q].first, queries[q].second);
			//cerr << res << '\n';
			ans += res;
		}
		cout << "Case #" << case_num << ": " << ans << '\n';
#endif

	}

	return 0;
}
