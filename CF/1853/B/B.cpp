#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	std::vector<int> fib;
	fib.push_back(0);
	fib.push_back(1);
	fib.reserve(128);

	int T; cin >> T;
	while (T--) {
		int N, K; cin >> N >> K;
		auto solve = [&]() -> int {
			while (int(fib.size()) < K+1) {
				if (fib.end()[-1] > N) {
					return 0;
				}
				fib.push_back(fib.end()[-2] + fib.end()[-1]);
			}
			int a = fib[K-1];
			int b = fib[K];
			// We want to count linear combinations of a and b which make N.
			// The crt is exactly fib[K-3] * b - fib[K-2] * a == +-1
			int64_t ca = (K & 1) ? fib[K-2] : fib[K-1];
			int64_t cb = (K & 1) ? fib[K-3] : fib[K-2];
			assert(ca * a - cb * b == 1);
			ca *= N, cb *= N;
			// ca * a - cb * b == N
			// we need to subtract some number of as from cb
			int64_t max_sub = (ca / b);
			int64_t min_sub = (cb + a - 1) / a;
			if (max_sub >= min_sub) {
				return int(max_sub - min_sub + 1);
			} else {
				return 0;
			}
		};
		cout << solve() << '\n';
	}

	return 0;
}
