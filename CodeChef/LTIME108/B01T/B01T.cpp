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

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N; cin >> N;
		std::vector<int> A(N);
		for (auto& a : A) cin >> a;

		std::vector<std::vector<int>> adj(N);
		for (int e = 0; e < N-1; e++) {
			int u, v; cin >> u >> v; u--, v--;
			adj[u].push_back(v);
			adj[v].push_back(u);
		}

		int tot = std::accumulate(A.begin(), A.end(), 0);
		int tgt = N / 2;

		std::vector<int> take(N);
		std::y_combinator([&](auto self, int cur, int prv) -> int {
			int cur_val = !A[cur] - A[cur];
			for (int nxt : adj[cur]) {
				if (nxt == prv) continue;
				int v = self(nxt, cur);
				cur_val += v;
			}
			if (std::abs(cur_val + tot - tgt) < std::abs(tot - tgt)) {
				take[cur] = true;
				tot += cur_val;
				cur_val = 0;
			}
			return cur_val;
		})(0, -1);

		std::vector<int> ans; ans.reserve(N);
		std::y_combinator([&](auto self, int cur, int prv) -> void {
			if (take[cur]) {
				ans.push_back(cur);
				return;
			}
			for (int nxt : adj[cur]) {
				if (nxt == prv) continue;
				self(nxt, cur);
			}
		})(0, -1);
		cout << ans.size() << '\n';
		for (int z = 0; z < int(ans.size()); z++) {
			cout << ans[z]+1 << " \n"[z+1==int(ans.size())];
		}
	}

	return 0;
}
