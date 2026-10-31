#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N, Q; cin >> N >> Q;
	std::vector<int> A(N);
	for (int i = 0; i < N; i++) {
		cin >> A[i];
	}
	std::sort(A.begin(), A.end());

	std::vector<std::pair<int64_t, int>> queries(Q);
	for (int q = 0; q < Q; q++) {
		cin >> queries[q].first; queries[q].second = q;
	}
	//std::sort(queries.begin(), queries.end());
	
	auto solve_query = [&](int64_t k) -> int64_t {
		int64_t cur_ans = 0;
		for (int b = 60; b >= 0; b--) {
			int64_t tgt = cur_ans | (int64_t(1) << b);
			int64_t ops_left = k;
			for (int i = 0; i < N; i++) {
				if ((tgt & ~A[i]) == 0) continue;
				int64_t first_mismatch = 63 - __builtin_clzll(tgt & ~A[i]);
				int64_t new_A = (A[i] & -(int64_t(1) << (first_mismatch + 1))) | tgt;
				ops_left -= new_A - A[i];
				if (ops_left < 0) break;
			}
			if (ops_left >= 0) cur_ans = tgt;
		}
		return cur_ans;
	};
	for (int q = 0; q < Q; q++) {
		cout << solve_query(queries[q].first) << '\n';
	}

	return 0;
}
