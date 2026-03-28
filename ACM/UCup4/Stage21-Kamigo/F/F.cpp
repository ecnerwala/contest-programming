#include <bits/stdc++.h>

int main() {
	std::ios_base::sync_with_stdio(false), std::cin.tie(nullptr);

	int N; std::cin >> N;
	std::vector<int> A(N);
	std::vector<int> B(N);
	for (auto& a : A) { std::cin >> a; }
	for (auto& b : B) { std::cin >> b; }
	std::vector<std::pair<int64_t, int>> c_vals(N);
	if ([&]() -> bool {
		if (A[0] != 1 && B[0] != 1) {
			return false;
		}
		c_vals[0].first = 0;
		std::vector<int64_t> stk; stk.reserve(N);
		int idx = 0;
		for (int i = 1; i < N; i++) {
			bool incr_lis = A[i] - A[i-1];
			bool incr_lds = B[i] - B[i-1];
			if (A[i] - A[i-1] != incr_lis) return false;
			if (B[i] - B[i-1] != incr_lds) return false;
			if (incr_lis && incr_lds) return false;
			if (incr_lis) {
				// LIS increases, so let's push
				c_vals[i].first = int64_t(A[i] - 1) * int64_t(N);
				for (int z = 1; z < B[i] && int(stk.size()) < N; z++) {
					stk.push_back(int64_t(A[i] - 1) * int64_t(N) - z);
				}
			} else if (incr_lds) {
				c_vals[i].first = -(B[i] - 1);
				for (int z = 1; z < A[i] && int(stk.size()) < N; z++) {
					stk.push_back(int64_t(z) * int64_t(N) - (B[i] - 1));
				}
			} else {
				if (idx == int(stk.size())) return false;
				c_vals[i].first = stk[idx++];
			}
		}
		return true;
	}()) {
		for (int i = 0; i < N; i++) c_vals[i].second = i;
		std::sort(c_vals.begin(), c_vals.end());
		std::vector<int> C(N);
		for (int i = 0; i < N; i++) C[c_vals[i].second] = i;
		for (int i = 0; i < N; i++) {
			std::cout << C[i]+1 << " \n"[i+1==N];
		}
	} else {
		std::cout << -1 << '\n';
	}

	return 0;
}
