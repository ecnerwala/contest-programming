#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	if (fopen("integral.in", "r")) {
		freopen("integral.in", "r", stdin);
		freopen("integral.out", "w", stdout);
	}

	int N; cin >> N;
	std::vector<std::array<int, 2>> pts(N);
	for (auto& [x, y] : pts) { cin >> x >> y; }
	std::vector<char> collinear(N);
	for (int i = 0; i < N; i++) {
		auto a = pts[i];
		auto b = pts[(i+1)%N];
		auto c = pts[(i+2)%N];
		c[0] -= b[0];
		c[1] -= b[1];
		a[0] -= b[0];
		a[1] -= b[1];
		collinear[i] = int64_t(a[1]) * int64_t(c[0]) == int64_t(a[0]) * int64_t(c[1]);
	}
	int64_t ans = 0;
	{
		{
			int i = 0;
			while (i < N && collinear[i]) i++;
			if (i == N) {
				cout << 0 << '\n';
				exit(0);
			}
			std::rotate(collinear.begin(), collinear.begin() + i+1, collinear.end());
		}
		assert(!collinear.back());
		for (int i = 0, j = 0; i < N; i = j+1) {
			j = i;
			while (collinear[j]) j++;
			cerr << i << ' ' << j << '\n';
			int num_segs = j - i + 1;
			ans -= int64_t(num_segs) * int64_t(num_segs + 1) / 2;
		}
	}

	for (auto& [x, y] : pts) { x &= 1; y &= 1; }
	std::vector<int> pref(N+1);
	for (int i = 0; i < N; i++) {
		pref[i+1] = pref[i] ^ (pts[i][0] & pts[(i+1)%N][1]) ^ (pts[i][1] & pts[(i+1)%N][0]);
	}
	if (pref[N] != 0) {
		cout << 0 << '\n';
		exit(0);
	} else {
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
		cout << ans << '\n';
	}

	return 0;
}

// integer area means that sum of x_i * y_{i+1} has the right parity
