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

		int cur = 1;
		int64_t ans = 0;
		for (int i = 0; i < N; i++) {
			assert(A[i] >= cur - 1);
			if (A[i] >= cur) {
				ans += A[i] - cur;
				cur++;
			}
		}
		cout << ans << '\n';
	}

	return 0;
}
