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

namespace ecnerwala {

using std::swap;

struct level_ancestor {
	int N;
	std::vector<int> depth;
	std::vector<int> preorder;
	std::vector<int> idx;
	std::vector<std::pair<int, int>> heavyPar; // heavy parent, distance
	level_ancestor() : N(0) {}

	level_ancestor(const std::vector<int>& par) : N(int(par.size())), depth(N), preorder(N), idx(N), heavyPar(N) {
		std::vector<std::vector<int>> ch(N);
		for (int i = 0; i < N; i++) {
			if (par[i] != -1) ch[par[i]].push_back(i);
		}
		std::vector<int> sz(N);
		int nxt_idx = 0;
		for (int i = 0; i < N; i++) {
			if (par[i] == -1) {
				depth[i] = 0;
				std::y_combinator([&](auto self, int cur) -> void {
					sz[cur] = 1;
					for (int nxt : ch[cur]) {
						depth[nxt] = depth[cur] + 1;
						self(nxt);
						sz[cur] += sz[nxt];
					}
					if (!ch[cur].empty()) {
						auto mit = max_element(ch[cur].begin(), ch[cur].end(), [&](int a, int b) { return sz[a] < sz[b]; });
						swap(*ch[cur].begin(), *mit);
					}
				})(i);
				std::y_combinator([&](auto self, int cur, int isRoot = true) -> void {
					preorder[idx[cur] = nxt_idx++] = cur;
					if (isRoot) {
						heavyPar[idx[cur]] = {par[cur] == -1 ? -1 : idx[par[cur]], 1};
					} else {
						assert(idx[par[cur]] == idx[cur]-1);
						heavyPar[idx[cur]] = heavyPar[idx[cur]-1];
						heavyPar[idx[cur]].second++;
					}
					bool chRoot = false;
					for (int nxt : ch[cur]) {
						self(nxt, chRoot);
						chRoot = true;
					}
				})(i);
			}
		}
	}

	int get_ancestor(int a, int k) const {
		assert(k >= 0);
		a = idx[a];
		while (a != -1 && k) {
			if (k >= heavyPar[a].second) {
				k -= heavyPar[a].second;
				assert(heavyPar[a].first <= a - heavyPar[a].second);
				a = heavyPar[a].first;
			} else {
				a -= k;
				k = 0;
			}
		}
		if (a == -1) return -1;
		else return preorder[a];
	}

	int lca(int a, int b) const {
		a = idx[a], b = idx[b];
		while (true) {
			if (a > b) swap(a, b);
			assert(a <= b);
			if (a > b - heavyPar[b].second) {
				return preorder[a];
			}
			b = heavyPar[b].first;
			if (b == -1) return -1;
		}
	}

	int dist(int a, int b) const {
		a = idx[a], b = idx[b];
		int res = 0;
		while (true) {
			if (a > b) swap(a, b);
			assert(a <= b);
			if (a > b - heavyPar[b].second) {
				res += b - a;
				break;
			}
			res += heavyPar[b].second;
			b = heavyPar[b].first;
			if (b == -1) return -1;
		}
		return res;
	}
};

} // namespace ecnerwala

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int Q; cin >> Q;
	const int VA = int(1e5);
	const int VB = int(1e5);
	const int V = VA + VB;
	std::vector<std::array<int, 2>> queries(Q);
	for (auto& [a, b] : queries) {
		cin >> a >> b; a--, b--; b += VA;
	}

	std::vector<std::vector<int>> adj(V);
	{
		std::vector<int> par(V, -1);
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
		for (auto [a, b] : queries) {
			if (merge(a, b)) {
				adj[a].push_back(b);
				adj[b].push_back(a);
			}
		}
	}

	std::vector<int> tree_par(V, -1);
	for (int z = 0; z < V; z++) {
		if (tree_par[z] == -1) {
			std::y_combinator([&](auto self, int cur, int prv) -> void {
				tree_par[cur] = prv;
				for (int nxt : adj[cur]) {
					if (nxt == prv) continue;
					self(nxt, cur);
				}
			})(z, -1);
		}
	}
	ecnerwala::level_ancestor la(tree_par);

	std::vector<int> par(V, -1);
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

	auto get_dist = [&](int a, int b) -> std::pair<int, std::array<int, 2>> {
		int d = la.dist(a, b) + (a >= VA) + (b >= VA);
		assert(d % 2 == 0);
		return {d, {a, b}};
	};

	std::array<int, 2> tot_ans{0, 0};
	struct comp_data_t {
		std::array<int, 2> cur_ans;
		std::array<int, 2> alive_freq;
		int bad_vert = -1;
		std::pair<int, std::array<int, 2>> diam;
	};
	auto update_tot_ans = [&](comp_data_t& d) -> void {
		assert((d.cur_ans == std::array<int, 2>{0, 0}));
		d.cur_ans = d.alive_freq;
		if (d.bad_vert == -1) {
			assert(d.diam.first % 2 == 0);
			d.cur_ans[(d.diam.first / 2) & 1] --;
		}
		tot_ans[0] += d.cur_ans[0];
		tot_ans[1] += d.cur_ans[1];
	};
	auto remove_tot_ans = [&](comp_data_t& d) -> void {
		tot_ans[0] -= d.cur_ans[0];
		tot_ans[1] -= d.cur_ans[1];
		d.cur_ans = {0, 0};
	};

	std::vector<comp_data_t> dat(V);
	for (int v = 0; v < V; v++) {
		dat[v].cur_ans = {0, 0};
		dat[v].alive_freq = {0, 0};
		dat[v].alive_freq[v >= VA]++;
		dat[v].diam = get_dist(v, v);
		dat[v].bad_vert = -1;
		update_tot_ans(dat[v]);
		assert(dat[v].cur_ans[0] == 0);
		assert(dat[v].cur_ans[1] == 0);
	}

	auto get_next = [&](int a, int r) -> int {
		assert(a != r);
		int l = la.lca(a, r);
		if (l == a) {
			return la.get_ancestor(r, la.depth[r] - la.depth[a] - 1);
		} else {
			return la.get_ancestor(a, 1);
		}
	};

	std::vector<bool> is_alive(V, true);

	auto mark_bad = [&](comp_data_t& d, int a) -> void {
		assert(d.bad_vert != -1);
		while (true) {
			if (!is_alive[a]) break;
			is_alive[a] = false;
			d.alive_freq[a >= VA] --;
			if (a == d.bad_vert) break;
			a = get_next(a, d.bad_vert);
		}
	};

	for (auto [a, b] : queries) {
		int pa = get_par(a);
		int pb = get_par(b);
		if (pa == pb) {
			auto& d = dat[pa];
			remove_tot_ans(d);
			if (d.bad_vert == -1) {
				d.bad_vert = a;
			}
			mark_bad(d, a);
			mark_bad(d, b);
			update_tot_ans(d);
		} else {
			auto& da = dat[pa];
			auto& db = dat[pb];
			remove_tot_ans(da);
			remove_tot_ans(db);
			if (da.bad_vert != -1 && db.bad_vert != -1) {
				mark_bad(da, a);
				mark_bad(db, b);
			}
			comp_data_t nd;
			nd.cur_ans = {0, 0};
			nd.alive_freq[0] = da.alive_freq[0] + db.alive_freq[0];
			nd.alive_freq[1] = da.alive_freq[1] + db.alive_freq[1];
			nd.bad_vert = (da.bad_vert != -1 ? da.bad_vert : db.bad_vert);
			nd.diam = std::max({
				get_dist(da.diam.second[0], db.diam.second[0]),
				get_dist(da.diam.second[0], db.diam.second[1]),
				get_dist(da.diam.second[1], db.diam.second[0]),
				get_dist(da.diam.second[1], db.diam.second[1]),
				da.diam, db.diam
			});
			merge(a, b);
			update_tot_ans(dat[get_par(a)] = nd);
		}
	}

	return 0;
}
