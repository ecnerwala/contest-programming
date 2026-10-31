#include<bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N, M; cin >> N >> M;
	vector<string> G(N); for (auto& s : G) cin >> s;
	vector<vector<int>> max_rect(N, vector<int>(M, N+M));

	for (int flip_x = 0; flip_x < 2; flip_x++) {
		for (int flip_y = 0; flip_y < 2; flip_y++) {
			vector<vector<int>> dp(N, vector<int>(M));
			for (int i = 0; i < N; i++) {
				for (int j = 0; j < M; j++) {
					if (i && j && G[i-1][j] == G[i][j] && G[i][j-1] == G[i][j]) {
						dp[i][j] = 1 + min(dp[i-1][j], dp[i][j-1]);
					} else {
						dp[i][j] = 0;
					}
					max_rect[i][j] = min(max_rect[i][j], dp[i][j]);
				}
			}
			for (auto& r : G) reverse(r.begin(), r.end());
			for (auto& r : max_rect) reverse(r.begin(), r.end());
		}
		reverse(G.begin(), G.end());
		reverse(max_rect.begin(), max_rect.end());
	}
	int64_t ans = 0;
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < M; j++) {
			ans += max_rect[i][j]+1;
		}
	}
	cout << ans << '\n';

	return 0;
}
