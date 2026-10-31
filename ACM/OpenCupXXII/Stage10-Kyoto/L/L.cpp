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

	int on_path(int st, int en, int l) {
		int m = lca(st, en);
		if (l <= dist(st, m)) {
			return get_ancestor(st, l);
		} else {
			return get_ancestor(en, dist(st, en) - l);
		}
	}
};

} // namespace ecnerwala

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N, Q; cin >> N >> Q;
	std::vector<std::vector<int>> adj(N);
	for (int e = 0; e < N-1; e++) {
		int u, v; cin >> u >> v; u--, v--;
		adj[u].push_back(v);
		adj[v].push_back(u);
	}

	std::vector<int> par(N);
	std::vector<std::pair<int, int>> furthest_down(N);
	std::y_combinator([&](auto self, int cur, int prv) -> void {
		par[cur] = prv;
		furthest_down[cur] = {0, cur};
		for (int nxt : adj[cur]) {
			if (nxt == prv) continue;
			self(nxt, cur);
			auto v = furthest_down[nxt];
			v.first++;
			furthest_down[cur] = max(furthest_down[cur], v);
		}
	})(0, -1);
	std::vector<std::pair<int, int>> furthest_up(N);
	std::vector<std::pair<int, int>> furthest(N);
	std::vector<std::set<int>> ch_dist(N);
	std::y_combinator([&](auto self, int cur, int prv) -> void {
		std::pair<int, int> furthest1{0, cur};
		std::pair<int, int> furthest2{-1, -1};
		for (int nxt : adj[cur]) {
			auto v = nxt == prv ? furthest_up[cur] : furthest_down[nxt];
			v.first++;
			ch_dist[cur].insert(v.first);
			if (v > furthest1) {
				furthest2 = furthest1;
				furthest1 = v;
			} else if (v > furthest2) {
				furthest2 = v;
			}
		}
		furthest[cur] = furthest1;
		for (int nxt : adj[cur]) {
			if (nxt == prv) continue;
			auto v = furthest_down[nxt];
			v.first++;
			if (v == furthest1) {
				furthest_up[nxt] = furthest2;
			} else {
				furthest_up[nxt] = furthest1;
			}
			self(nxt, cur);
		}
	})(0, -1);
	/*
	cerr << "global" << '\n';
	for (int i = 0; i < N; i++) {
		cerr << i << ' ' << furthest[i].first << ' ' << furthest[i].second << '\n';
	}
	cerr << "down" << '\n';
	for (int i = 1; i < N; i++) {
		cerr << i << ' ' << furthest_down[i].first << ' ' << furthest_down[i].second << '\n';
	}
	cerr << "up" << '\n';
	for (int i = 1; i < N; i++) {
		cerr << i << ' ' << furthest_up[i].first << ' ' << furthest_up[i].second << '\n';
	}
	*/

	ecnerwala::level_ancestor anc(par);

	auto solve_query = [&](int v, int d) {
		//cerr << "solve_query " << v << ' ' << d << '\n';
		assert(d > 0);
		assert(furthest[v].first >= d);
		int turn_pt = anc.on_path(v, furthest[v].second, (d-1)/2);
		int ans = d;
		//cerr << "turn_pt " << turn_pt << '\n';
		if (d > 2) {
			int anc_pt = anc.on_path(v, furthest[v].second, (d-1)/2 - 1);
			//cerr << "anc_pt " << anc_pt << '\n';
			int l;
			if (anc_pt == par[turn_pt]) {
				l = furthest_up[turn_pt].first;
			} else if (par[anc_pt] == turn_pt) {
				l = furthest_down[anc_pt].first;
			} else assert(false);
			assert(l >= ((d-1)/2 - 1));
			//cerr << "len " << l << '\n';
			ans = std::max(ans, d + (l - ((d-1)/2 - 1)));;
		}
		if (d % 2 == 0 && ch_dist[turn_pt].count(d/2)) {
			ans = std::max(ans, d+1);
		}
		return ans;
	};

	for (int q = 0; q < Q; q++) {
		int v, d; cin >> v >> d; v--;
		cout << solve_query(v, d) << '\n';
	}

	return 0;
}
