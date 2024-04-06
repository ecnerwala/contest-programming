#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N, M, K; cin >> N >> M >> K;
		std::vector<int> A(N);
		for (auto& a : A) cin >> a;
		std::sort(A.begin(), A.end());
		int64_t ans = int64_t(K) * int64_t(K-1) / 2;
		for (int i = 0; i < N; i++) {
			int c = std::min(K, M);
			K -= c;
			ans += int64_t(A[i]) * int64_t(c) - int64_t(c) * int64_t(c-1) / 2;
		}
		cout << ans << '\n';
	}

	return 0;
}
