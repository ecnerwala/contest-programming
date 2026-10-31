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

	int N; cin >> N;
	vector<vector<int>> adj(N);
	for (int e = 0; e < N-1; e++) {
		int a, b; cin >> a >> b; a--, b--;
		adj[a].push_back(b);
		adj[b].push_back(a);
	}
	vector<int> sz(N);
	std::y_combinator([&](auto self, int cur, int prv) -> void {
		sz[cur] = 1;
		for (int nxt : adj[cur]) {
			if (nxt == prv) continue;
			self(nxt, cur);
			sz[cur] += sz[nxt];
		}
	})(0, -1);
	int root = 0;
	while (true) {
		for (int nxt : adj[root]) {
			if (sz[nxt] >= sz[root]) continue;
			if (sz[nxt] * 2 >= N) {
				root = nxt;
				goto found;
			}
		}
		break;
found:;
	}

	vector<int> par(N);
	vector<int> depth(N);
	vector<vector<int>> evts(N/2+1);
	std::y_combinator([&](auto self, int cur, int prv) -> void {
		depth[cur] = (prv == -1) ? 0 : depth[prv]+1;
		par[cur] = prv;
		sz[cur] = 1;
		for (int nxt : adj[cur]) {
			if (nxt == prv) continue;
			self(nxt, cur);
			sz[cur] += sz[nxt];
		}

		if (prv != -1) {
			evts[sz[cur]].push_back(cur);
		}
	})(root, -1);

	ecnerwala::level_ancestor la(par);
	auto get_dist = [&](int a, int b) -> int {
		return depth[a] + depth[b] - 2 * depth[la.lca(a, b)];
	};

	array<int, 2> cur_diam{root, root};
	int cur_diam_len = 0;

	vector<int> ans(N+1);
	for (int i = N; i >= 1; i--) {
		if (i & 1) {
			ans[i] = 0;
			continue;
		}
		for (int v : evts[i / 2]) {
			int c0 = get_dist(cur_diam[0], v);
			int c1 = get_dist(cur_diam[1], v);
			if (c0 < c1) {
				swap(cur_diam[0], cur_diam[1]);
				swap(c0, c1);
			}
			assert(c0 >= c1);
			if (c0 > cur_diam_len) {
				cur_diam_len = c0;
				cur_diam = {cur_diam[0], v};
			}
		}
		ans[i] = cur_diam_len;
	}

	for (int i = 1; i <= N; i++) {
		cout << ans[i]+1 << '\n';
	}

	return 0;
}
