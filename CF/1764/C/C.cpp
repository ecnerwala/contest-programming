#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N; cin >> N;
		std::vector<int> A(N);
		for (auto& a : A) cin >> a;

		std::sort(A.begin(), A.end());
		
		int64_t ans = N/2;
		for (int i = 1; i < N; i++) {
			if (A[i-1] < A[i]) {
				ans = std::max(ans, int64_t(i) * int64_t(N - i));
			}
		}
		cout << ans << '\n';
	}

	return 0;
}
