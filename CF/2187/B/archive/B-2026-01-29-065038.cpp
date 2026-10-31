#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int X, Y; cin >> X >> Y;
		// casework on the direction of p, q wrt x, y
		using val_t = std::optional<std::pair<int64_t, std::array<int, 2>>>;
		val_t ans = std::nullopt;
		for (int dx = 0; dx < 2; dx++) {
			for (int dy = 0; dy < 2; dy++) {
				std::array<std::array<val_t, 2>, 2> dp;
				dp[0][0] = {0, {0, 0}};
				dp[0][1] = std::nullopt;
				dp[1][0] = std::nullopt;
				dp[1][1] = std::nullopt;
				for (int b = 30; b >= 0; b--) {
					int bx = (X >> b) & 1;
					int by = (Y >> b) & 1;
					std::array<std::array<val_t, 2>, 2> ndp;
					for (int cx = 0; cx < 2; cx++) {
						for (int cy = 0; cy < 2; cy++) {
							if (!dp[cx][cy]) continue;
							auto [w, ps] = *dp[cx][cy];
							for (int vx = 0; vx < 2; vx++) {
								for (int vy = 0; vy < 2; vy++) {
									if (!cx && (vx != bx) && (vx > bx) != dx) continue;
									if (!cy && (vy != by) && (vy > by) != dy) continue;
									int nx = cx || (vx != bx);
									int ny = cy || (vy != by);
									int64_t nw = w * 2 + (dx ? vx - bx : bx - vx) + (dy ? vy - by : by - vy);
									std::array<int, 2> nps = {ps[0] * 2 + vx, ps[1] * 2 + vy};
									auto& nv = ndp[nx][ny];
									if (!nv || nw < nv->first) nv = {nw, nps};
								}
							}
						}
					}
					dp = ndp;
				}
				if (dp[1][1]) {
					if (!ans || dp[1][1]->first < ans->first) ans = dp[1][1];
				}
			}
		}
		assert(ans);
		cerr << ans->first << '\n';
		cout << ans->second[0] << ' ' << ans->second[1] << '\n';
	}

	return 0;
}
