#include <bits/stdc++.h>

template <typename T> void setmin(T& a, T b) { if (b < a) a = b; }

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);
	int N, K; cin >> N >> K;
	vector<array<int64_t, 2>> bounds(N);
	// make them both exclusive
	for (auto& a : bounds) {
		cin >> a[0] >> a[1];
		a[0]--, a[1]++;
	}
	vector<int64_t> costs(K); for (auto& c : costs) cin >> c;

	const int64_t INF = 1e18;
	vector<array<array<int64_t, 4>, 4>> dp((N+1)*(N+2)/2);
	auto I = [](int i, int j) {
		return j * (j+1) / 2 + (j - i - 1);
	};
	for (int k = K; k >= 0; k--) {
		for (int j = 0; j <= N; j++) {
			for (int i = j-1; i >= -1; i--) {
				for (int mi = 3; mi >= 0; mi--) {
					for (int mj = 3; mj >= 0; mj--) {
						int64_t val;
						if (k == K) {
							val = (j-i == 1) ? 0 : INF;
						} else {
							bool needs = i >= 0 && j <= N-1 && bool((((bounds[i][mi>>1] ^ bounds[j][mj>>1]) >> k) ^ mi ^ mj) & 1);
							val = dp[I(i,j)][mi&2][mj&2] + (needs * costs[k]);
						}
						if ((mi & 1) && (mj & 1)) {
							// we get to skip this part
						} else {
							for (int md = i+1; md <= j-1; md++) {
								for (int z = 0; z < 2; z++) {
									// we can try using this midpoint
									int64_t mm = (bounds[md][z] >> k) ^ 1;
									if (bounds[md][0] < (mm << k) && ((mm + 1) << k) <= bounds[md][1]) {
										setmin(val, dp[I(i,md)][mi][2*z+1] + dp[I(md,j)][2*z+1][mj]);
									}
								}
							}
						}
						dp[I(i,j)][mi][mj] = val;
					}
				}

			}
		}
	}

	cout << dp.back()[0][0] << '\n';

	return 0;
}
