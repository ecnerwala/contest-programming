#include <bits/stdc++.h>

int64_t solve(int64_t N) {
	assert(N > 0);
	const int MX = 1000;
	const int L = 6;
	auto power = [&](int64_t a, int64_t e) -> int64_t {
		int64_t r = 1;
		while (e--) r *= a;
		return r;
	};
	assert(power(MX+1, L) > N);
	for (int K = 3; K <= MX; K++) {
		if ([&]() -> bool {
			for (int64_t v = N; v; v /= K) if (v % K > 1) return false;
			return true;
		}()) {
			return K;
		}
	}
	int64_t best = -1;
	for (int msk = (1 << L) - 1; msk >= 0; msk--) {
		int64_t lo = 0;
	}
	return best;
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
