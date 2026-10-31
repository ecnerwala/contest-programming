#include "tickets.h"
#include <bits/stdc++.h>

long long find_maximum(int K, std::vector<std::vector<int>> X) {
	int N = int(X.size());
	int M = int(X[0].size());

	std::vector<std::vector<int>> answer(N, std::vector<int>(M, -1));
	struct cnd_t {
		int val;
		int i;
	};

	std::vector<cnd_t> cnds; cnds.reserve(N*K);

	int64_t tot_val = 0;
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < K; j++) {
			tot_val -= X[i][j];
			cnds.push_back({X[i][j] + X[i][j+(M-K)], i});
		}
	}

	auto md = cnds.begin() + N/2*K;
	std::nth_element(cnds.begin(), md, cnds.end(),
			[&](const auto& a, const auto& b) { return a.val > b.val; });
	cnds.erase(md, cnds.end());

	std::vector<int> num_hi(N, 0);
	for (auto [v, i] : cnds) {
		tot_val += v;
		num_hi[i]++;
	}

	for (int k = K-1; k >= 0; k--) {
		int c0 = N/2;
		int c1 = N/2;
		for (int i = 0; i < N; i++) {
			if (num_hi[i] == k+1) {
				c1--;
			} else if (num_hi[i] == 0) {
				c0--;
			} else {
				// do nothing
			}
		}
		assert(c0 <= N/2 && c1 <= N/2);
		for (int i = 0; i < N; i++) {
			int d;
			if (num_hi[i] == k+1) {
				d = 1;
			} else if (num_hi[i] == 0) {
				d = 0;
			} else if (c0) {
				c0--;
				d = 0;
			} else if (c1) {
				c1--;
				d = 1;
			} else assert(false);
			if (d) {
				answer[i][M - (num_hi[i]--)] = k;
			} else {
				answer[i][k - num_hi[i]] = k;
			}
		}
	}

	allocate_tickets(answer);
	return tot_val;
}
