#include "supertrees.h"
#include <bits/stdc++.h>

int construct(std::vector<std::vector<int>> P) {
	int N = int(P.size());
	std::vector<std::vector<int>> answer(N, std::vector<int>(N, 0));

	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			assert(P[i][j] == P[j][i]);
			assert(0 <= P[i][j] && P[i][j] <= 3);
			if (P[i][j] == 3) return false;
		}
	}

	std::vector<bool> is_root(N, false);
	// check rectangles of 1's
	for (int i = 0; i < N; i++) {
		assert(P[i][i] == 1);

		int cc = 0;
		while (P[i][cc] != 1) cc++;

		if (cc != i) {
			if (P[i] != P[cc]) {
				return false;
			}

			answer[i][cc] = answer[cc][i] = true;
		} else {
			is_root[i] = true;
		}
	}

	// check rectangles of nonzero's
	for (int i = 0; i < N; i++) {
		if (!is_root[i]) continue;

		int cc = 0;
		while (P[i][cc] == 0) cc++;
		for (int j = 0; j < N; j++) {
			if ((P[i][j] == 0) != (P[cc][j] == 0)) {
				return false;
			}
		}

		if (cc == i) {
			int prv = i;
			for (int j = i+1; j < N; j++) {
				if (is_root[j] && P[i][j]) {
					answer[prv][j] = answer[j][prv] = true;
					prv = j;
				}
			}
			if (prv == i) {
				// do nothing
			} else if (answer[i][prv]) {
				return false;
			} else {
				answer[i][prv] = answer[prv][i] = true;
			}
		}
	}

	build(answer); return true;
}
