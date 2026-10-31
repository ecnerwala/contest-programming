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
	std::vector<int> preorder;
	std::vector<int> idx;
	std::vector<std::pair<int, int>> heavyPar; // heavy parent, distance
	level_ancestor() : N(0) {}

	level_ancestor(const std::vector<int>& par) : N(int(par.size())), preorder(N), idx(N), heavyPar(N) {
		std::vector<std::vector<int>> ch(N);
		for (int i = 0; i < N; i++) {
			if (par[i] != -1) ch[par[i]].push_back(i);
		}
		std::vector<int> sz(N);
		int nxt_idx = 0;
		for (int i = 0; i < N; i++) {
			if (par[i] == -1) {
				std::y_combinator([&](auto self, int cur) -> void {
					sz[cur] = 1;
					for (int nxt : ch[cur]) {
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
};

} // namespace ecnerwala

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N, Q; cin >> N >> Q;
	vector<vector<int>> adj(N);
	for (int e = 0; e < N-1; e++) {
		int u, v; cin >> u >> v; u--, v--;
		adj[u].push_back(v);
		adj[v].push_back(u);
	}

	vector<int> par(N);
	vector<int> depth(N);
	std::y_combinator([&](auto self, int cur, int prv = -1) -> void {
		depth[cur] = (prv == -1 ? 0 : depth[prv] + 1);
		par[cur] = prv;
		for (int nxt : adj[cur]) {
			if (nxt == prv) continue;
			self(nxt, cur);
		}
	})(0);

	ecnerwala::level_ancestor lcaer(par);

	using path_t = pair<int, int>;
	using diam_t = pair<int, array<path_t, 2>>;
	diam_t NO_DIAM{int(1e9), array<path_t, 2>{path_t{-1, -1}, {-1, -1}}};

	auto lca = [&](int a, int b) {
		return lcaer.lca(a, b);
	};

	auto dist = [&](int a, int b) {
		return depth[a] + depth[b] - 2 * depth[lca(a,b)];
	};

	auto get_diam = [&](path_t a, path_t b) -> diam_t {
		int ca = lca(a.first, a.second);
		int cb = lca(b.first, b.second);
		if (depth[ca] > depth[cb]) {
			swap(ca, cb);
			swap(a, b);
		}
		assert(depth[ca] <= depth[cb]);
		// we just need pt-path distance now
		int l = dist(ca, cb);
		for (int p : {a.first, a.second}) {
			int d = depth[lca(p, cb)];
			if (d >= depth[ca]) {
				l = min(l, depth[cb] - d);
			}
		}
		return diam_t{l, {a, b}};
	};

	auto update_diam = [&](diam_t d, path_t a) {
		if (d == NO_DIAM) return get_diam(a, a);
		return max({d, get_diam(a, d.second[0]), get_diam(a, d.second[1])});
	};

	set<pair<path_t, int>> alive;
	vector<vector<path_t>> ops(2*Q);
	vector<int> queries(Q, -1);

	for (int q = 0; q < Q; q++) {
		int op; cin >> op;
		if (op == 1 || op == 2) {
			int u, v; cin >> u >> v; u--, v--;
			if (u > v) swap(u, v);
			path_t path(u, v);
			if (op == 1) {
				alive.insert({path, q});
			} else {
				auto it = alive.lower_bound({path, -1});
				assert(it != alive.end() && it->first == path);
				int t0 = it->second;
				alive.erase(it);

				for (int a = Q+t0, b = Q+q; a < b; a >>= 1, b >>= 1) {
					if (a & 1) ops[a++].push_back(path);
					if (b & 1) ops[--b].push_back(path);
				}
			}
		} else if (op == 3) {
			int d; cin >> d;
			queries[q] = d * 2;
		} else assert(false);
	}
	for (auto it : alive) {
		int t0 = it.second;
		path_t path = it.first;
		for (int a = Q+t0, b = Q+Q; a < b; a >>= 1, b >>= 1) {
			if (a & 1) ops[a++].push_back(path);
			if (b & 1) ops[--b].push_back(path);
		}
	}

	vector<int> actual_diam(Q);
	std::y_combinator([&](auto self, int a, diam_t d) -> void {
		//cerr << "push " << a << '\n';
		for (path_t p : ops[a]) {
			//cerr << "adding " << p.first << '-' << p.second << '\n';
			d = update_diam(d, p);
			//cerr << "got " << d.first << '\n';
		}
		if (a >= Q) {
			actual_diam[a-Q] = d.first;
			return;
		}
		self(2*a, d);
		self(2*a+1, d);
		//cerr << "pop " << a << '\n';
	})(1, NO_DIAM);

	for (int q = 0; q < Q; q++) {
		if (queries[q] == -1) continue;
		if (actual_diam[q] <= queries[q]) {
			cout << "Yes" << '\n';
		} else {
			cout << "No" << '\n';
		}
	}

	return 0;
}

// If A has diameter x..y
//
// Then, A + {i} has diameter x..y, x..i, or i..y
