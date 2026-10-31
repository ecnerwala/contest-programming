#include "mosaic.h"

#include <bits/stdc++.h>

std::vector<long long> mosaic(
	std::vector<int> X0, std::vector<int> Y0,
	std::vector<int> X_lo, std::vector<int> X_hi,
	std::vector<int> Y_lo, std::vector<int> Y_hi
) {
	int N = int(X0.size());
	assert(N == int(Y0.size()));
	int Q = int(X_lo.size());
	assert(Q == int(X_hi.size()));
	assert(Q == int(Y_lo.size()));
	assert(Q == int(Y_hi.size()));

	assert(X0[0] == Y0[0]);

	auto f = [](int a, int b) -> int {
		return (a == 0) && (b == 0) ? 1 : 0;
	};

	std::vector<int> prefX0(N);
	std::vector<int> prefY0(N);
	for (int i = 1; i < N; i++) {
		prefX0[i] = prefX0[i-1] + X0[i];
		prefY0[i] = prefY0[i-1] + Y0[i];
	}

	std::vector<int> X1(N-1);
	std::vector<int> Y1(N-1);
	if (N > 1) {
		{
			int prv = Y0[1];
			for (int i = 1; i < N; i++) {
				prv = (X1[i-1] = f(prv, X0[i]));
			}
		}
		{
			int prv = X0[1];
			for (int i = 1; i < N; i++) {
				prv = (Y1[i-1] = f(prv, Y0[i]));
			}
		}
	}

	std::vector<std::pair<int, int64_t>> pref_x_max(2*(N-1));
	std::vector<std::pair<int, int64_t>> pref_y_max(2*(N-1));
	for (int x = 0; x < N-1; x++) {
		int y = 0;
		int diag = x - y + (N-2);
		int val = Y1[x];
		if (val == 1) {
			//std::cerr << "diag " << diag << ' ' << x << ' ' << y << '\n';
			pref_x_max[diag + 1].second++;
			pref_y_max[diag + 1].second++;
		}
	}
	// y = 1 to avoid doublecounting the middle
	for (int y = 1; y < N-1; y++) {
		int x = 0;
		int diag = x - y + (N-2);
		int val = X1[y];
		if (val == 1) {
			//std::cerr << "diag " << diag << ' ' << x << ' ' << y << '\n';
			pref_x_max[diag + 1].second++;
			pref_y_max[diag + 1].second++;
		}
	}
	if (N > 2) {
		std::vector<int> X2(N-2);
		{
			int prv = Y1[1];
			for (int i = 1; i < N-1; i++) {
				prv = (X2[i-1] = f(prv, X1[i]));
			}
		}
		std::vector<int> Y2(N-2);
		{
			int prv = X1[1];
			for (int i = 1; i < N-1; i++) {
				prv = (Y2[i-1] = f(prv, Y1[i]));
			}
		}
		assert(X2[0] == Y2[0]);
		for (int x = 0; x < N-2; x++) {
			int y = 0;
			int diag = x - y + (N-2);
			int val = Y2[x];
			if (val == 1) {
				//std::cerr << "diag " << diag << ' ' << x << ' ' << y << '\n';
				pref_x_max[diag + 1].first++;
				pref_x_max[diag + 1].second -= (x+1);
				pref_y_max[diag + 1].first++;
				pref_y_max[diag + 1].second -= (y+1);
			}
		}
		// y = 1 to avoid doublecounting the middle
		for (int y = 1; y < N-2; y++) {
			int x = 0;
			int diag = x - y + (N-2);
			int val = X2[y];
			if (val == 1) {
				//std::cerr << "diag " << diag << ' ' << x << ' ' << y << '\n';
				pref_x_max[diag + 1].first++;
				pref_x_max[diag + 1].second -= (x+1);
				pref_y_max[diag + 1].first++;
				pref_y_max[diag + 1].second -= (y+1);
			}
		}
	}
	for (int i = 0; i+1 < 2 * (N-1); i++) {
		pref_x_max[i+1].first += pref_x_max[i].first;
		pref_x_max[i+1].second += pref_x_max[i].second;
		pref_y_max[i+1].first += pref_y_max[i].first;
		pref_y_max[i+1].second += pref_y_max[i].second;
	}

	auto query = [&](int x_max, int y_max) -> int64_t {
		assert(-1 <= x_max && x_max <= N-1);
		assert(-1 <= y_max && y_max <= N-1);
		if (x_max == -1 && y_max == -1) return X0[0];
		else if (x_max == -1) return -prefX0[y_max];
		else if (y_max == -1) return -prefY0[x_max];

		assert(0 <= x_max && x_max <= N-1);
		assert(0 <= y_max && y_max <= N-1);
		if (x_max == 0 || y_max == 0) return 0;
		int lo = 1 - y_max + (N-2);
		int md = x_max - y_max + (N-2);
		int hi = x_max - 1 + (N-2) + 1;
		int64_t v = (
			0
			+ (pref_x_max[hi].first - pref_x_max[md].first) * int64_t(x_max)
			+ (pref_x_max[hi].second - pref_x_max[md].second)
			+ (pref_y_max[md].first - pref_y_max[lo].first) * int64_t(y_max)
			+ (pref_y_max[md].second - pref_y_max[lo].second)
		);
		//std::cerr << x_max << ' ' << y_max << ' ' << v << '\n';
		return v;
	};

	std::vector<long long> ans(Q, 0);
	for (int q = 0; q < Q; q++) {
		int x_lo = X_lo[q];
		int x_hi = X_hi[q];
		int y_lo = Y_lo[q];
		int y_hi = Y_hi[q];
		x_lo--, y_lo--;
		ans[q] = query(x_hi, y_hi) - query(x_lo, y_hi) - query(x_hi, y_lo) + query(x_lo, y_lo);
	}

	return ans;
}
