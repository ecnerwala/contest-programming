#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N, L, R; cin >> N >> L >> R;
		std::vector<int> A(N);
		for (auto& a : A) cin >> a;
		std::sort(A.begin(), A.end());
		int64_t ans = 0;
		for (int i = 0, j = N-1; i < N; i++, j--) {
			int lo = std::min(R, A[i]);
			int hi = std::max(L, A[j]);
			if (lo >= hi) break;
			ans += hi - lo;
		}
		cout << ans << '\n';
	}

	return 0;
}
