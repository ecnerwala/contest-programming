#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N, M, K; cin >> N >> M >> K;
		// This matrix equal values on each diagonal
		int res = 0;
		for (int s = 2; s <= N+M; s++) {
			int cnt = min({s-1, N+M+1-s, N, M});
			if (cnt & 1) res ^= K+s;
		}
		cout << res << '\n';
	}

	return 0;
}
