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

	int T; cin >> T;
	while (T--) {
		int L, M; cin >> L >> M;
		std::vector<int> A(L); for (auto& a : A) { cin >> a; a--; }
		std::vector<int> B(M); for (auto& b : B) { cin >> b; b--; }
		//cerr << "Case" << '\n';
		//cerr << L << ' ' << M << '\n';
		//for (auto& b : B) { cerr << b << ' '; } cerr << '\n';

		std::vector<int> root_par(M, -1);
		std::vector<std::vector<int>> ch(M);
		std::vector<int> tree_par(M, -1);
		{
			std::vector<int> par(M, -1);
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
			for (int i = 0; i < M; i++) {
				if (!merge(i, B[i])) {
					root_par[i] = B[i];
				} else {
					tree_par[i] = B[i];
					ch[B[i]].push_back(i);
				}
			}
		}

		std::vector<int> depth(M, -1);
		std::vector<int> root_jump(M, -1);
		std::vector<std::array<int, 2>> st(M);
		{
			int idx = 0;
			for (int rt = 0; rt < M; rt++) {
				if (root_par[rt] == -1) continue;
				depth[rt] = 1;
				std::y_combinator([&](auto self, int cur) -> void {
					st[cur][0] = idx;
					idx++;
					root_jump[cur] = root_par[rt];
					for (int nxt : ch[cur]) {
						depth[nxt] = depth[cur] + 1;
						self(nxt);
					}
					st[cur][1] = idx;
				})(rt);
			}
			assert(idx == M);
		}

		auto can_reach = [&](int src, int dest, int max_steps) -> bool {
			if (st[dest][0] <= st[src][0] && st[src][0] < st[dest][1]) {
				// in the subtree
				return depth[dest] - depth[src] <= max_steps;
			} else {
				max_steps -= depth[src];
				src = root_jump[src];
				if (st[dest][0] <= st[src][0] && st[src][0] < st[dest][1]) {
					// in the subtree
					return depth[dest] - depth[src] <= max_steps;
				} else {
					return false;
				}
			}
		};

		int mi = -1;
		int ma = M;
		while (ma - mi > 1) {
			int md = (mi + ma) / 2;
			int cur = 0;
			for (auto a : A) {
				while (!can_reach(a, cur, md)) {
					cur++;
					if (cur == M) break;
				}
				if (cur == M) break;
			}
			if (cur == M) {
				mi = md;
			} else {
				ma = md;
			}
		}
		if (ma == M) {
			// M-1 steps lets you get everything, so M is never necessary
			cout << -1 << '\n';
		} else {
			cout << ma << '\n';
		}
	}

	return 0;
}
