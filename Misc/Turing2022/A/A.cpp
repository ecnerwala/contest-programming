#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N; int64_t M; cin >> N >> M;
		std::vector<int> A(N);
		for (auto& a : A) cin >> a;

		std::map<int, int64_t> dp;
		dp[0] = 0;
		for (int i = 0; i < N; i++) {
			std::map<int, int64_t> ndp;
			auto ins = [&](int k, int64_t v) {
				auto it = ndp.find(k);
				if (it == ndp.end()) ndp.insert({k, v});
				else it->second = std::min(it->second, v);
			};

			const int64_t INF = 1e18;

			int64_t v0 = INF;
			for (auto it : dp) {
				ins(it.first | A[i], it.second);
				v0 = std::min(v0, it.second + int64_t(it.first | A[i]) - M);
			}
			if (i+1==N) cout << v0 << '\n';
			ins(0, v0);
			dp = ndp;
		}
	}

	return 0;
}
