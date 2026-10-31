#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int64_t N; cin >> N;

	auto is_pow_2 = [](int64_t v) { return (v & (v - 1)) == 0; };

	if (N % 4 == 3) {
		// the xor of 1..N is exactly 0
		cout << N << '\n';
	} else if (N % 4 == 2) {
		// xor of 1..N is N+1, can't make with deleting 0
		// xor of 1..N+1 is 0, can't make with deleting 1
		// xor of 1..N+2 is N+2, need to make with deleting 2
		if (!is_pow_2(N+2)) {
			cout << N+2 << '\n';
			cout << ((N+2) & -(N+2)) << ' ' << ((N+2) & (N+1)) << '\n';
		} else {
			// need to make 1 with deleting 3
			assert(N >= 6);
			cout << N+3 << '\n';
			cout << 2 << ' ' << 5 << ' ' << 6 << '\n';
		}
	} else if (N % 4 == 1) {
		assert(N >= 5);
		// xor of 1..N is 1, can't make with deleting 0
		// xor of 1..N+1 is N+2, can't make with deleting 1
		// xor of 1..N+2 is 0, can't make with deleting 2
		// xor of 1..N+3 is N+3, need to make with deleting 3
		if (!is_pow_2(N+3)) {
			assert(N+3 >= 12);
			cout << N+3 << '\n';
			int64_t lo = (N+3) & -(N+3);
			int64_t hi = (N+3) & (N+2);
			cout << 1 << ' ' << (lo + 1) << ' ' << hi << '\n';
		} else {
			// xor of 1..N+4 is 1
			cout << N+4 << '\n';
			cout << 1 << ' ' << 2 << ' ' << 4 << ' ' << 6 << '\n';
		}
	} else if (N % 4 == 0) {
		// xor of 1..N is N, can't make with deleting 0
		// xor of 1..N+1 is 1
		cout << N+1 << '\n';
		cout << 1 << '\n';
	} else assert(false);

	return 0;
}
