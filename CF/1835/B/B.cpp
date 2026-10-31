#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N; cin >> N;
	int64_t M; cin >> M;
	int K; cin >> K;
	K = std::min(K, N+1);

	std::vector<int64_t> A(N);
	for (auto& a : A) {
		cin >> a;
	}

	std::sort(A.begin(), A.end());

	// you lose if it's at least (you + (Kth)-guy) / 2 or at most (you - (Kth)-guy) / 2
	int64_t best_cnt = -1;
	int64_t best_pick = -1;
	auto check = [&](int64_t c, int64_t p) {
		if (c > best_cnt || (c == best_cnt && p < best_pick)) {
			best_cnt = c;
			best_pick = p;
		}
	};
	auto check_cnd = [&](int64_t p, int i, int j) -> void {
		// i is lower bound, j is upper bound
		assert(0 <= i && i <= j && j <= N);
		assert(K >= 1);
		int64_t lo = (j-K >= 0 ? (p + A[j-K]) / 2 : -1);
		int64_t hi = (i+K-1 < N ? (p + A[i+K-1] + 1) / 2 : M+1);
		check(std::max<int64_t>(0, hi - lo - 1), p);
	};
	for (int i = 0, j = 0; i < N; i = j) {
		while (j < N && A[j] == A[i]) j++;
		// check equal to A[i]
		check_cnd(A[i], i, j);
	}

	for (int i = 0; i <= N; i++) {
		int64_t lo = (i == 0) ? 0 : A[i-1]+1;
		int64_t hi = (i == N) ? M : A[i]-1;
		if (lo <= hi) {
			// check this candidate
			for (int z = 0; z <= 5 && z <= hi-lo; z++) {
				check_cnd(lo + z, i, i);
				check_cnd(hi - z, i, i);
			}
		}
	}

	assert(best_cnt != -1);
	cout << best_cnt << ' ' << best_pick << '\n';

	return 0;
}
