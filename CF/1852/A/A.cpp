#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N, K; cin >> N >> K;
		std::vector<int> A(N); for (auto& a : A) cin >> a;

		int64_t ans = 1;
		for (int i = 0; i <= N; i++) {
			int num_days = K;
			if (i < N) {
				int64_t diff = (A[i] - i) - ans;
				if (diff <= 0) {
					num_days = 0;
				} else if (i > 0) {
					int64_t delta_t = (diff + i - 1) / i;
					if (delta_t < num_days) {
						num_days = int(delta_t);
					}
				}
			}
			ans += int64_t(num_days) * int64_t(i);
			K -= num_days;
		}
		cout << ans << '\n';
	}

	return 0;
}
