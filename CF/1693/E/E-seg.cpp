#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N; cin >> N;
	std::vector<int> A(N);
	for (auto& a : A) cin >> a;

	const int V = N;
	std::vector<std::vector<int>> idxs(V+1);
	for (int i = 0; i < N; i++) {
		idxs[A[i]].push_back(i);
	}

	std::vector<int> seg(2*N);

	int cur_lo = 0;
	int cur_hi = N;
	int64_t ans = 0;
	for (int v = V; v > 0; v--) {
		if (idxs[v].empty()) continue;

		int nxt_lo = std::min(idxs[v].front(), cur_hi);
		int nxt_hi = std::max(idxs[v].back()+1, cur_lo);
		cur_lo = nxt_lo;
		cur_hi = nxt_hi;

		for (int a : idxs[v]) {
			for (a += N; a; a >>= 1) seg[a]++;
		}

		// [cur_lo, cur_hi)
		for (int a = cur_lo+N, b = cur_hi+N; a < b; a >>= 1, b >>= 1) {
			if (a & 1) ans += seg[a++];
			if (b & 1) ans += seg[--b];
		}
	}
	cout << ans << '\n';

	return 0;
}
