#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N; cin >> N;
		std::vector<int> A(N);
		for (auto& a : A) cin >> a;
		int64_t ans = 0;
		for (int i = 1; i < N-1; i++) {
			if (A[i] > A[i-1] && A[i] > A[i+1]) {
				int l = i;
				while (l > 0 && A[l] > A[l-1]) l--;
				int r = i;
				while (r < N-1 && A[r] > A[r+1]) r++;
				ans += int64_t(r - i) * int64_t(i - l);
			}
		}
		cout << ans << '\n';
	}

	return 0;
}
