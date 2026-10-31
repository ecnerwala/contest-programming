#include <bits/stdc++.h>
using namespace std;

template <typename T> void setmin(T& a, T b) { if (b < a) a = b; }

int main() {
	ios_base::sync_with_stdio(0), cin.tie(nullptr);

	int N, K; int64_t W, S; cin >> N >> K >> W >> S;
	vector<int64_t> T(N); for (auto& t : T) cin >> t;

	int64_t ans = W * ((N+K-1)/K);

	const int64_t INF = 1e18;

	vector<int64_t> dp(K);
	vector<int64_t> ndp(K);
	for (int i = 0; i < N; i++) {
		std::fill(ndp.begin(), ndp.end(), INF);
		for (int a = 0; a < K; a++) {
			setmin(ndp[a], dp[a] + T[i] + S * min(a, K-a));
			if (a+1==K) {
				setmin(ndp[0], dp[a] + W);
			} else {
				setmin(ndp[a+1], dp[a]);
			}
		}
		std::swap(dp, ndp);
	}
	setmin(ans, dp[0]);

	cout << ans << '\n';
}
