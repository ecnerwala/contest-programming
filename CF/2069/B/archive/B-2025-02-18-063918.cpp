#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N, M; cin >> N >> M;
		std::vector<std::array<bool, 2>> has(N*M, {false, false});
		for (int i = 0; i < N; i++) {
			for (int j = 0; j < M; j++) {
				int a; cin >> a; a--;
				has[a][(i^j)&1] = true;
			}
		}
		int ans = 0;
		int mx = 0;
		for (int i = 0; i < N*M; i++) {
			ans += has[i][0] + has[i][1];
			mx = std::max(mx, has[i][0] + has[i][1]);
		}
		cout << ans - mx << '\n';
	}

	return 0;
}
