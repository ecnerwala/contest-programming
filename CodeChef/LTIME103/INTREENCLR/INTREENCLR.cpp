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

constexpr bool TESTING = false;

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);
	int T; cin >> T;
	while (T--) {
		int N, Q; cin >> N >> Q;
		std::vector<std::vector<int>> adj(N);
		for (int e = 0; e < N-1; e++) {
			int u, v; cin >> u >> v; u--, v--;
			adj[u].push_back(v);
			adj[v].push_back(u);
		}

		mt19937 mt(48);
		std::vector<int> C(N);
		if constexpr (TESTING) {
			for (auto& v : C) cin >> v;
		}

		auto query = [&](int x) {
			if constexpr (TESTING) {
				if (x == -1) {
					for (int i = 1; i < N; i++) {
						assert(C[i] == C[i-1]);
					}
					return 1;
				}
				C[x] ^= 1;
				int ans = 0;
				for (int i = 0; i < N; i++) {
					for (int j : adj[i]) ans += C[i] ^ C[j];
				}
				assert(!(ans & 1));
				return ans >> 1;
			}
			cout << x+1 << '\n' << flush;
			int r; cin >> r;
			if (r == -1) exit(0);
			return r;
		};

		std::vector<int> dp(N);
		int last = query(0);
		std::y_combinator([&](auto self, int cur, int prv) -> int {
			int v = 0;
			if (prv != -1) {
				v = query(cur) - last;
				last += v;
			}
			for (int nxt : adj[cur]) {
				if (nxt == prv) continue;
				v += self(nxt, cur);
			}
			if (prv != -1) {
				assert(v == 1 || v == -1);
				dp[cur] = -v;
			}
			return v;
		})(0, -1);

		dp[0] = 1;
		std::vector<int> black; black.reserve(N);
		std::vector<int> white; white.reserve(N);
		std::y_combinator([&](auto self, int cur, int prv) -> void {
			if (prv != -1) {
				dp[cur] *= dp[prv];
			}
			(dp[cur] == -1 ? black : white).push_back(cur);
			for (int nxt : adj[cur]) {
				if (nxt == prv) continue;
				self(nxt, cur);
			}
		})(0, -1);
		if (black.size() < white.size()) std::swap(white, black);

		for (int v : white) query(v);

		query(-1);
	}

	return 0;
}
