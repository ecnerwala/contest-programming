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
};

} // namespace ecnerwala

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N; cin >> N;
	std::vector<std::vector<std::pair<int, int>>> adj(N);
	for (int e = 0; e < N-1; e++) {
		int u, v; cin >> u >> v; u--, v--;
		adj[u].push_back({v, e});
		adj[v].push_back({u, e});
	}

	std::vector<int64_t> D(N-1);
	for (auto& d : D) cin >> d;

	std::vector<int64_t> W(N-1);

	std::vector<int64_t> depth(N);
	std::vector<int> lcas(N-1);
	{
		std::vector<int> par(N, -1);
		std::y_combinator([&](auto self, int cur, int prv) -> void {
			par[cur] = prv;
			for (auto [nxt, e] : adj[cur]) {
				if (nxt == prv) continue;
				self(nxt, cur);
			}
		})(0, -1);
		ecnerwala::level_ancestor lca(par);
		for (int i = 0; i+1 < N; i++) {
			lcas[i] = lca.lca(i, i+1);
		}
	}

	std::vector<int> cur_parity(N);
	for (int b = 0; b < 60; b++) {
		int64_t cur_bit = int64_t(1) << b;
		cur_parity[0] = 0;
		for (int i = 0; i+1 < N; i++) {
			int64_t cur_d = D[i] - (depth[i] + depth[i+1] - 2 * depth[lcas[i]]);
			if (cur_d < 0) {
				cout << -1 << '\n';
				exit(0);
			}
			assert((cur_d & (cur_bit - 1)) == 0);
			cur_parity[i+1] = cur_parity[i] ^ ((cur_d >> b) & 1);
		}
		depth[0] = 0;
		std::y_combinator([&](auto self, int cur, int prv) -> void {
			for (auto [nxt, e] : adj[cur]) {
				if (nxt == prv) continue;
				W[e] += int64_t(cur_parity[cur] ^ cur_parity[nxt]) << b;
				depth[nxt] = depth[cur] + W[e];
				self(nxt, cur);
			}
		})(0, -1);
	}

	for (int i = 0; i < N-1; i++) {
		if (D[i] != depth[i] + depth[i+1] - 2 * depth[lcas[i]]) {
			cout << -1 << '\n';
			exit(0);
		}
	}
	for (int e = 0; e < N-1; e++) {
		if (W[e] == 0) {
			cout << -1 << '\n';
			exit(0);
		}
	}
	for (int e = 0; e < N-1; e++) {
		cout << W[e] << '\n';
	}

	return 0;
}
