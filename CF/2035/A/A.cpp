#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int64_t N, M, R, C; cin >> N >> M >> R >> C;
		int64_t ans = N * M - (R-1) * M + C + (N-R) * (M-1);
		cout << ans << '\n';
	}

	return 0;
}
