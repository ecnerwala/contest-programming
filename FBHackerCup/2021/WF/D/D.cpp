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

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	for (int case_num = 1; case_num <= T; case_num ++) {
		cerr << "Case #" << case_num << "\n";

		int N; int64_t K; cin >> N >> K;
		std::vector<int64_t> C(N);
		for (auto& c : C) cin >> c;

		std::vector<std::vector<int>> adj(N);
		for (int i = 1; i < N; i++) {
			int p; cin >> p; p--;
			adj[i].push_back(p);
			adj[p].push_back(i);
		}

		int ROOT = std::y_combinator([&](auto self, int cur, int prv) -> std::pair<int64_t, int> {
			std::pair<int64_t, int> res{-1, -1};
			for (int nxt : adj[cur]) {
				if (nxt == prv) continue;
				res = std::max(res, self(nxt, cur));
			}
			if (res.first == -1) res = {0, cur};
			res.first += C[cur];
			return res;
		})(0, -1).second;

		std::vector<std::pair<int64_t, int>> dp(N);
		std::y_combinator([&](auto self, int cur, int prv) -> std::pair<int64_t, int> {
			std::pair<int64_t, int> res{-1, -1};
			for (int nxt : adj[cur]) {
				if (nxt == prv) continue;
				res = std::max(res, self(nxt, cur));
			}
			if (res.first == -1) res = {0, cur};
			res.first += C[cur];
			return dp[cur] = res;
		})(ROOT, -1);
		for (int i = 0; i < N; i++) {
			assert(dp[i].first <= dp[ROOT].first);
		}
		std::sort(dp.begin(), dp.end(), std::greater<>());

		int64_t cur = 0;
		int num_leaves = 1; // the root
		std::vector<bool> used(N);
		for (auto [v, l] : dp) {
			if (used[l]) continue;
			used[l] = true;
			num_leaves++;
			cur += v;
			if (cur >= K) break;
		}


		cout << "Case #" << case_num << ": ";
		if (cur < K) {
			cout << -1 << '\n';
		} else {
			cout << (num_leaves+1) / 2 - 1 << '\n';
		}
	}

	return 0;
}
