#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int64_t K, J; int N; cin >> K >> J >> N;
	int64_t j0 = std::lcm(J, K);
	if (j0 > N) {
		cout << N/J + N/K << '\n';
		exit(0);
	}

	if (std::gcd(J, K) > 1) {
		cout << std::min(N/J + (N+1)/K, (N+1)/J + N/K) << '\n';
		exit(0);
	}

	std::array<std::pair<int64_t, int64_t>, 2> jump_times;
	for (int i = 1; true; i++) {
		if (i * K % J == 1) {
			jump_times[0] = {i*K-1, i + (i*K/J)};
			break;
		}
	}
	for (int i = 1; true; i++) {
		if (i * J % K == 1) {
			jump_times[1] = {i*J-1, i + (i*J/K)};
			break;
		}
	}

	int64_t ans = N+1;
	std::vector<int64_t> dp(N, N+1);
	dp[j0 - 1] = j0 / J + j0 / K;
	for (int cur = 0; cur < N; cur++) {
		if (dp[cur] > N) continue;
		for (int z = 0; z < 2; z++) {
			auto [l, cnt] = jump_times[z];
			if (cur + l < N) {
				dp[cur + l] = std::min(dp[cur + l], dp[cur] + cnt);
			} else {
				if (z == 0) {
					ans = std::min(ans, dp[cur] + (N-1-cur) / J + (N-cur) / K);
				} else {
					ans = std::min(ans, dp[cur] + (N-cur) / J + (N-1-cur) / K);
				}
			}
		}
	}
	cout << ans << '\n';

	return 0;
}
