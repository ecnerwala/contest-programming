#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N; cin >> N;
	std::vector<int64_t> A(N);
	for (auto& a : A) cin >> a;
	int64_t max_k = 0;
	int64_t idx = -1;
	for (int i = 0; i+1 < N; i++) {
		auto v = (A[i+1] ^ A[i]);
		assert(v > 0 && (v & (v+1)) == 0);
		// we flipped k bits, which means that (L + i+1) == 2^k-1 mod 2^{k}
		int k = __builtin_ctzll(v+1);
		assert(k > 0);
		if (k > max_k) {
			max_k = k;
			idx = i;
		}
	}

	// lo bits of L
	int64_t lo_bits = 0;
	if (max_k == 0) {
		lo_bits = 0;
	} else {
		// L + idx + 1 == 2^{max_k-1} mod 2^max_k
		lo_bits = ((int64_t(1) << (max_k-1)) - 1 - idx) & ((int64_t(1) << max_k) - 1);
	}

	// lo bits of s
	cout << ((lo_bits ^ A[0]) & ((int64_t(1) << max_k) - 1)) << '\n';


	return 0;
}
