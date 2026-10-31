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

template <typename T> void setmax(T& a, T b) { if (b > a) a = b; }
template <typename T> void setmin(T& a, T b) { if (b < a) a = b; }

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	for (int case_num = 1; case_num <= T; case_num ++) {
		int N; cin >> N;
		std::vector<int> C(N);
		for (auto& c : C) {
			cin >> c;
		}

		std::vector<std::vector<int>> adj(N);
		for (int e = 0; e < N-1; e++) {
			int u, v; cin >> u >> v; u--, v--;
			adj[u].push_back(v);
			adj[v].push_back(u);
		}

		int best1 = 0;
		int best2 = 0;
		const int ROOT = 0;
		for (int c : adj[ROOT]) {
			int v = std::y_combinator([&](auto self, int cur, int prv) -> int {
				int r = 0;
				for (int nxt : adj[cur]) {
					if (nxt == prv) continue;
					setmax(r, self(nxt, cur));
				}
				r += C[cur];
				return r;
			})(c, ROOT);
			if (v > best1) best2 = best1, best1 = v;
			else if (v > best2) best2 = v;
		}

		int ans = C[ROOT] + best1 + best2;

		cout << "Case #" << case_num << ": " << ans << '\n';
	}

	return 0;
}
