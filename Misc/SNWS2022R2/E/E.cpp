#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N; cin >> N;
	std::vector<std::array<int, 2>> A(N);
	for (auto& a : A) cin >> a[0] >> a[1];
	std::sort(A.begin(), A.end());

	std::array<std::vector<int>, 8> nxt;
	for (int z = 0; z < 8; z++) nxt[z].assign(N, -1);

	for (int i = 0; i < N; i++) {
		for (int j = i+1; j < N; j++) {
			int dx = A[j][0] - A[i][0];
			int dy = A[j][1] - A[i][1];
			if (dx == -dy) {
				if (nxt[0][i] == -1) {
					nxt[0][i] = j, nxt[4][j] = i;
				}
			} else if (dy == 0) {
				if (nxt[1][i] == -1) {
					nxt[1][i] = j, nxt[5][j] = i;
				}
			} else if (dx == dy) {
				if (nxt[2][i] == -1) {
					nxt[2][i] = j, nxt[6][j] = i;
				}
			} else if (dx == 0) {
				if (nxt[3][i] == -1) {
					nxt[3][i] = j, nxt[7][j] = i;
				}
			}
		}
	}

	int64_t ans = 0;
	std::vector<int64_t> dp(N);
	std::vector<int64_t> ndp(N);
	for (int i = 0; i < N; i++) {
		std::fill(dp.begin(), dp.end(), 0);
		dp[i] = 1;
		for (int z = 0; z < 4; z++) {
			std::fill(ndp.begin(), ndp.end(), 0);
			for (int j = i; j < N; j++) {
				if (nxt[z][j] != -1) {
					ndp[nxt[z][j]] += dp[j];
					dp[nxt[z][j]] += dp[j];
				}
			}
			std::swap(dp, ndp);
		}
		for (int z = 4; z < 8; z++) {
			std::fill(ndp.begin(), ndp.end(), 0);
			for (int j = N-1; j >= i; j--) {
				if (nxt[z][j] != -1) {
					ndp[nxt[z][j]] += dp[j];
					dp[nxt[z][j]] += dp[j];
				}
			}
			std::swap(dp, ndp);
		}
		ans += dp[i];
	}
	cout << ans << '\n';

	return 0;
}
