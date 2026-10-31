#include "bulb.h"

#include <bits/stdc++.h>
using namespace std;

int FindWinner(int T, std::vector<int> L, std::vector<int> R){
	assert(T >= 1);
	int N = int(L.size());
	vector<bool> hasPath(N);
	vector<bool> hasPathChange(N);

	vector<bool> hasRed;

	auto dfs_impl = [&](auto dfs, int cur) -> void {
		assert(cur >= 0);

		if (L[cur] >= 0) {
			dfs(dfs, L[cur]);
		}
		if (R[cur] >= 0) {
			dfs(dfs, R[cur]);
		}

		hasPath[cur] = L[cur] < 0 ? L[cur] == -2 : hasPath[L[cur]];

		hasPathChange[cur] =
			(R[cur] < 0 ? R[cur] == -2 : hasPath[R[cur]])
			|| (L[cur] < 0 ? false : hasPathChange[L[cur]]);
	};

	dfs_impl(dfs_impl, 0);
	return 0;
}
