#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N; cin >> N;
		int val;
		std::vector<int> P;
		if (N & 1) {
			// if N is odd, the last value is a mask, so it's an upper bound
			// end should be N, N-1, odd, odd
			if (N >= 5) {
				val = N;
				P.resize(N);
				std::iota(P.begin(), P.end(), 1);
				std::swap(P[N-5], P[N-4]);
			} else {
				assert(false);
			}
		} else {
			// if N is even, the last value is a free OR, so we can set it to N
			// we want 0b10000, 0b011111, 0b011110, odd, odd
			int v = 31 - __builtin_clz(N);
			assert(N >= (1 << v));
			assert(N < (2 << v));
			val = (2 << v) - 1;
			std::iota(P.begin(), P.end(), 1);
			std::rotate(P.begin(), P.begin() + (1 << v), P.end());
			std::swap(P[N-6], P[N-5]);
		}
		cout << val << '\n';
		for (int i = 0; i < N; i++) {
			cout << P[i] << " \n"[i+1==N];
		}
	}

	return 0;
}
