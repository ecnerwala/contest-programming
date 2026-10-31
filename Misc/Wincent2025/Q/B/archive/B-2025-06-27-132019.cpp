#include <bits/stdc++.h>

int64_t solve(int64_t N) {
	for (int K = 3; K <= 1000; K++) {
		if ([&]() -> bool {
			for (int64_t v = N; v; v /= K) if (v % K > 1) return false;
			return true;
		}()) {
			return K;
		}
	}
	return -1;
}

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int64_t N; cin >> N;
		cout << solve(N) << '\n';
	}

	return 0;
}
