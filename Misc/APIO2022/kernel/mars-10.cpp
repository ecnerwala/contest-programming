#include "mars.h"

#include <bits/stdc++.h>

long long process(std::vector<std::vector<long long>> a, int cur_x, int cur_y, int K, int N) {
	using namespace std;
	assert(int(a.size()) == 3);
	for (auto& r : a) assert(int(r.size()) == 3);

	/*
	if (i > N - 1 - K) {
		// reflect and do the sub problem
		std::reverse(a.begin(), a.end());
		return process(a, 2 * (N - 1 - K) - i, j, K, N);
	}
	assert(i <= N - 1 - K);
	if (j > N - 1 - K) {
		for (auto& r : a) std::reverse(r.begin(), r.end());
		return process(a, i, 2 * (N - 1 - K) - j, K, N);
	}
	assert(j <= N - 1 - K);
	*/

	int L = 2 * N + 1;
	int prv_L = 2 * K + 1;
	int nxt_L = prv_L + 2;

	// solution for <= 10

	if (nxt_L == L) {
		std::vector<std::vector<int>> grid(L, std::vector<int>(L));
		int small_L = 1;
		while (small_L * 3 < L) small_L += 2;

		for (int i = 0; i < L; i++) {
			for (int j = 0; j < L; j++) {
				int i_x = 0;
				int i_off = i;
				while (i_off >= small_L) i_x++, i_off -= (N - small_L / 2);

				int j_x = 0;
				int j_off = j;
				while (j_off >= small_L) j_x++, j_off -= (N - small_L / 2);

				grid[i][j] = (a[i_x][j_x] >> (i_off * small_L + j_off)) & 1;
			}
		}

		std::vector<int> par(L * L, -1);
		auto get_par = [&](int a) -> int {
			while (par[a] >= 0) {
				if (par[par[a]] >= 0) par[a] = par[par[a]];
				a = par[a];
			}
			return a;
		};
		auto merge = [&](int a, int b) -> bool {
			a = get_par(a);
			b = get_par(b);
			if (a == b) return false;
			if (par[a] > par[b]) std::swap(a, b);
			par[a] += par[b];
			par[b] = a;
			return true;
		};

		int ans = 0;
		for (int i = 0; i < L; i++) {
			for (int j = 0; j < L; j++) {
				ans += grid[i][j];
			}
		}
		for (int i = 0; i < L; i++) {
			for (int j = 0; j < L; j++) {
				if (i+1 < L && grid[i][j] && grid[i+1][j]) {
					ans -= merge(i*L+j, (i+1)*L+j);
				}
				if (j+1 < L && grid[i][j] && grid[i][j+1]) {
					ans -= merge(i*L+j, i*L+(j+1));
				}
			}
		}
		return ans;
	} else if (prv_L * 3 < L) {
		assert(nxt_L * nxt_L <= 49);
		// expand our reach
		int64_t res = 0;
		for (int i = 0; i < nxt_L; i++) {
			for (int j = 0; j < nxt_L; j++) {
				int i_x = 0;
				int i_off = i;
				while (i_off >= prv_L) i_x++, i_off --;

				int j_x = 0;
				int j_off = j;
				while (j_off >= prv_L) j_x++, j_off --;

				bool v = (a[i_x][j_x] >> (i_off * prv_L + j_off)) & 1;

				res += int64_t(v) << (i * nxt_L + j);
			}
		}
		return res;
	} else {
		int zi = (cur_x >= N - K - 1) + (cur_x > N - K - 1);
		int zj = (cur_y >= N - K - 1) + (cur_y > N - K - 1);
		return a[zi][zj];
	}
}
