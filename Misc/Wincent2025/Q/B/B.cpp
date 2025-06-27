#include <bits/stdc++.h>

int64_t solve(int64_t N) {
	//std::cerr << "solve " << N << '\n';
	assert(N > 0);
	const int MX = 1000;
	assert(MX >= 3);
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
	for (int msk = (1 << L) - 1; msk > 1; msk--) {
		//std::cerr << "msk " << msk << '\n';
		auto eval = [&](int64_t K) -> int64_t {
			//std::cerr << "  check " << K << '\n';
			int64_t r = 0;
			for (int i = L-1; i >= 0; i--) {
				r *= K;
				if (msk & (1 << i)) r++;
			}
			return r;
		};
		int64_t lo = MX;
		int64_t hi = MX+1;
		while (eval(hi) < N) {
			std::tie(lo, hi) = std::make_pair(hi, hi + 2 * (hi - lo));
		}
		assert(eval(hi) >= N);
		while (hi - lo > 1) {
			int64_t mid = lo + (hi - lo) / 2;
			if (eval(mid) < N) lo = mid;
			else hi = mid;
		}
		assert(lo == MX || eval(lo) < N);
		assert(eval(hi) >= N);
		if (eval(hi) == N) {
			if (best == -1) best = hi;
			else if (hi < best) {
				assert(false);
				best = hi;
			}
		}
	}
	return best;
}

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int64_t N; cin >> N;
		int64_t r = solve(N);
		if (r == -1) {
			assert(N == 2);
		} else {
			for (int64_t v = N; v; v /= r) assert(v % r == 0 || v % r == 1);
		}
	}

	return 0;
}
