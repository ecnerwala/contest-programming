#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N; cin >> N;
		std::vector<int> A(N); for (auto& a : A) cin >> a;
		int mx = 0;
		int64_t ans = 0;
		for (int i = 1; i < N; i++) {
			int delta = std::max(A[i], A[i-1]) - A[i];
			A[i] += delta;
			ans += delta;
			mx = std::max(mx, delta);
		}
		ans += mx;
		cout << ans << '\n';
	}

	return 0;
}
