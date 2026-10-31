#include <bits/stdc++.h>

bool tm(int64_t a) {
	return __builtin_popcountll(a) & 1;
}

int64_t solve(int64_t L, int64_t R) {
	if (L == R) {
		int v = tm(L);
		return v == 1 ? 1 : 0;
	}
	if (R - L == 1) {
		int a = tm(L);
		int b = tm(R);
		if (a == b) goto solve_recurse;
		return a == 1 ? 2 : 0;
	}
	if (R - L == 2) {
		int a = tm(L);
		int b = tm(L+1);
		int c = tm(L+2);
		if (a == b || b == c) goto solve_recurse;
		return a == 1 ? 2 : 3;
	}
solve_recurse:
	return (solve(L>>1, R>>1)<<1) + (L & 1);
}

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int64_t L, R; cin >> L >> R;
		cout << solve(L, R) << '\n';
	}

	return 0;
}
