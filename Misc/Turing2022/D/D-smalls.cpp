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

/**
 * manacher(S): return the maximum palindromic substring of S centered at each point
 *
 * Input: string (or vector) of length N (no restrictions on character-set)
 * Output: vector res of length 2*N+1
 *   For any 0 <= i <= 2*N:
 *   * i % 2 == res[i] % 2
 *   * the half-open substring S[(i-res[i])/2, (i+res[i])/2) is a palindrome of length res[i]
 *   * For odd palindromes, take odd i, and vice versa
 */
template <typename V> std::vector<int> manacher(const V& S) {
	int N = int(S.size());
	std::vector<int> res(2*N+1, 0);
	for (int i = 1, j = -1, r = 0; i < 2*N; i++, j--) {
		if (i > r) {
			r = i+1, res[i] = 1;
		} else {
			res[i] = res[j];
		}
		if (i+res[i] >= r) {
			int b = r>>1, a = i-b;
			while (a > 0 && b < N && S[a-1] == S[b]) {
				a--, b++;
			}
			res[i] = b-a, j = i, r = b<<1;
		}
	}
	return res;
}

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N, Q; cin >> N >> Q;

	std::vector<std::vector<std::pair<int, char>>> adj(N);
	for (int e = 0; e < N-1; e++) {
		int u, v; char c; cin >> u >> v >> c; u--, v--;
		adj[u].push_back({v, c});
		adj[v].push_back({u, c});
	}

	std::vector<std::array<int, 2>> queries(Q);
	for (auto& q : queries) {
		int u, v; cin >> u >> v; u--, v--;
		q = {u, v};
	}

	std::vector<int> ans(Q, -1);

	std::string buf;
	for (int q = 0; q < Q; q++) {
		std::y_combinator([&](auto self, int cur, int prv) -> void {
			if (cur == queries[q][1]) {
				auto man = manacher(buf);
				int res = 0;
				for (int i = 1; i < int(man.size()); i++) {
					res += man[i] == i;
				}
				ans[q] = res;
			}
			for (auto e : adj[cur]) {
				int nxt = e.first; char c = e.second;
				if (nxt == prv) continue;
				buf.push_back(c);
				self(nxt, cur);
				buf.pop_back();
			}
		})(queries[q][0], -1);
	}

	for (int q = 0; q < Q; q++) {
		cout << ans[q] << '\n';
	}

	return 0;
}
