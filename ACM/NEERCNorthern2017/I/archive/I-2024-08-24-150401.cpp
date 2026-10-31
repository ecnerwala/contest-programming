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
		for (int i = 0; i < N; i++) {
			for (int j = i+2; j < N; j++) {
				if (i == 0 && j == N-1) {
					continue;
				}
				bool b = pref[j] ^ pref[i] ^ (pts[i][0] & pts[j][1]) ^ (pts[i][1] & pts[j][0]);
				ans += !b;
			}
		}
		cout << ans << '\n';
	}

	return 0;
}

// integer area means that sum of x_i * y_{i+1} has the right parity
