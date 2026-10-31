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
	for (int case_num = 1; case_num <= T; case_num ++) {

		int N; cin >> N;
		std::vector<std::vector<int>> adj(N);
		for (int e = 0; e < N-1; e++) {
			int a, b; cin >> a >> b; a--, b--;
			adj[a].push_back(b);
			adj[b].push_back(a);
		}
		std::vector<int> par(N);
		std::y_combinator([&](auto self, int cur, int prv) -> void {
			par[cur] = prv;
			for (int nxt : adj[cur]) {
				if (nxt == prv) continue;
				self(nxt, cur);
			}
		})(0, -1);

		ecnerwala::level_ancestor la(par);

		std::vector<int> weight(N);

		std::vector<int> first_freq(N, -1);
		std::vector<int> last_freq(N, -1);
		for (int i = 0; i < N; i++) {
			int f; cin >> f; f--;
			if (first_freq[f] == -1) first_freq[f] = i;
			int j = std::exchange(last_freq[f], i);
			if (j != -1) {
				weight[i]++;
				weight[j]++;
				weight[la.lca(i, j)] -= 2;
			}
		}

		for (int f = 0; f < N; f++) {
			int i = first_freq[f];
			int j = last_freq[f];
			if (i != -1) {
				weight[i]++;
				weight[j]++;
				weight[la.lca(i, j)] -= 2;
			}
		}

		int ans = 0;
		int root_sum = std::y_combinator([&](auto self, int cur, int prv) -> int {
			int res = weight[cur];
			for (int nxt : adj[cur]) {
				if (nxt == prv) continue;
				int v = self(nxt, cur);
				ans += (v == 0);
				res += v;
			}
			assert(res >= 0);
			return res;
		})(0, -1);
		assert(root_sum == 0);

		cout << "Case #" << case_num << ": " << ans << '\n';
	}

	return 0;
}
