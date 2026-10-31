#include <bits/stdc++.h>

int64_t solve(int64_t N, int64_t K) {
	int64_t offset = 0;
	while (true) {
		assert(N >= 2);
		if (N == 2) return (K ^ 1) + offset;
		assert(N >= 3);
		int highest_pow_2 = 63 - __builtin_clzll(N-2);
		if (N == (1ll << highest_pow_2) * 2 + 1) {
			if (K == 0) return 1 + offset;
			else if (K == N-2) return N-1 + offset;
			else if (K % 2 == 1) return K+2 + offset;
			else if (K % 2 == 0) return K-2 + offset;
			else assert(false);
		} else {
			int64_t M = N - (1ll << highest_pow_2);
			if (K < M) {
				offset += 0;
			} else if (K >= N - M) {
				offset += N - M;
				K -= (N-M);
			} else {
				return K + offset;
			}
			N = M;
		}
	}
}

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	/*
	std::vector<int> A({1, 0});
	for (int N = 3; N <= 100; N++) {
		std::vector<int> NA(N);
		NA[0] = A[0];
		for (int i = 1; i < N; i++) {
			int p = A[i-1]+1;
			NA[i] = (p==N-1 ? N-1 : A[p]);
		}
		A = std::move(NA);
		for (int i = 0; i < N; i++) {
			cerr << A[i] << " \n"[i+1==N];
		}
		for (int i = 0; i < N; i++) {
			assert(A[i] == solve(N, i));
		}
	}
	*/

	int T; cin >> T;
	while (T--) {
		int64_t N, K; cin >> N >> K;
		K--;
		cout << solve(N, K)+1 << '\n';
	}

	return 0;
}
