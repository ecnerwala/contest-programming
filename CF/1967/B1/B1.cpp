#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N, M; cin >> N >> M;

		// A+B is a multiple of B^2
		int64_t ans = 0;
		for (int64_t B = 1; B <= M && B * (B-1) <= N; B++) {
			ans += (N + B) / B / B;
			if (B == 1) ans--;
		}
		cout << ans << '\n';
	}

	return 0;
}
