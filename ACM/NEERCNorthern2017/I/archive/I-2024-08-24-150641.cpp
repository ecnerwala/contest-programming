#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N; cin >> N;
	std::vector<std::array<int, 2>> pts(N);
	for (auto& [x, y] : pts) { cin >> x >> y; x &= 1; y &= 1; }
	std::vector<int> pref(N+1);
	for (int i = 0; i < N; i++) {
		pref[i+1] = pref[i] ^ (pts[i][0] & pts[(i+1)%N][1]) ^ (pts[i][1] & pts[(i+1)%N][0]);
	}
	if (pref[N] != 0) {
		cout << 0 << '\n';
		exit(0);
	} else {
		int ans = 0;
		std::array<int, 8> dp; dp.fill(0);
		for (int i = 0; i < N; i++) {
			for (int x = 0; x < 2; x++) {
				for (int y = 0; y < 2; y++) {
					for (int p = 0; p < 2; p++) {
						bool b = p ^ pref[i] ^ (x & pts[i][1]) ^ (y & pts[i][0]);
						if (!b) ans += dp[x * 4 + y * 2 + p];
					}
				}
			}
			{
				int p = pref[i];
				int x = pts[i][0];
				int y = pts[i][1];
				dp[x * 4 + y * 2 + p]++;
			}
		}
		// TODO: count the number of collinear segmens
		ans -= N;
		cout << ans << '\n';
	}

	return 0;
}

// integer area means that sum of x_i * y_{i+1} has the right parity
