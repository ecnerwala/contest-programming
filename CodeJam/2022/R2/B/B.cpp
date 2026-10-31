#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	for (int case_num = 1; case_num <= T; case_num ++) {
		int64_t R; cin >> R;

		int64_t ans = 0;
		for (int64_t x = 1, y = R; x <= R; x++) {
			while (x * x + y * y > R * R + R) y--;
			ans += y;
		}
		for (int64_t r = 1, x = 0; r <= R; r++) {
			while ((x+1) * (x+1) + (x+2)*(x+2) - (x+2) < r * r) {
				x++;
			}
			int64_t cnt = x * 2;
			if ((x+1) * (x+1) + (x+1) * (x+1) - (x+1) < r * r) {
				cnt++;
			}
			ans -= cnt;
		}

		ans *= 4;
		cout << "Case #" << case_num << ": " << ans << '\n';
	}

	return 0;
}


// correct: draw everything such that x * x + y * y <= R * R + R + 0.5
//
// incorrect: draw such that x * x + y * y - y + 0.25 <= r * r <= x * x + y * y + y + 0.25 for integer r
// we know that x * x + y * y < r * r + y <= R * R + R, so this is a subset of the full circle
// We just want to know the number of points so that x * x + y * y - max(x, y) + 1 .. x * x + y * y + max(x, y) does not include a perfect square
// Each point has a unique radius, so we can just count.
