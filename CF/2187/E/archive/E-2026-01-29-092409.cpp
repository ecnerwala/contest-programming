#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N; cin >> N;
		std::vector<int> A(N);
		for (auto& a : A) cin >> a;
		std::string S; cin >> S;
		// the number of keys we carry is at most O(sqrt(max(A) + N)) since it takes that much time
		int T_INF = N+1 + *std::max_element(A.begin(), A.end());
		std::vector<int> dp; dp.reserve(N+1);
		dp.push_back(0);
		for (int i = 0; i < N; i++) {
			if (S[i] == '1') {
				// can pick up the key for free
				dp.insert(dp.begin(), dp[0]);
			}
			// Now, wait until we can get through the door
			{
				for (int k = 0; k < int(dp.size()); k++) {
					// we can wait
					dp[k] = std::max(A[i], dp[k]);
					// or we can use a key
					if (k+1 < int(dp.size())) dp[k] = std::min(dp[k], dp[k+1]);
				}
			}
			// walk through the door with all our keys
			for (int k = 0; k < int(dp.size()); k++) {
				dp[k] += 1 + 2 * std::max(k-1, 0);
			}
			cerr << "at " << i+1 << '\n';
			for (auto v : dp) { cerr << v << ' '; } cerr << '\n';
			while (dp.back() > T_INF) dp.pop_back();
			cout << dp[0] << " \n"[i+1==N];
		}
	}

	return 0;
}
