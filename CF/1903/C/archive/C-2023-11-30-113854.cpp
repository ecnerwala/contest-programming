#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N; cin >> N;
		std::vector<int64_t> A(N);
		for (auto& a : A) cin >> a;
		int64_t cur = 0;
		int64_t ans = 0;
		for (int i = N-1; i > 0; i--) {
			cur += A[i];
			if (cur > 0) ans += cur;
		}
		ans += A[0] + cur;
		cout << ans << '\n';
	}

	return 0;
}
