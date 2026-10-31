#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N; cin >> N;
		std::vector<int64_t> A(N);
		for (auto& a : A) cin >> a;

		int64_t ans = 0;
		for (int i = 0; i < N; i++) {
			int j = i+1;
			for (int k = i+2; k < N; k++) {
				while (j+1 < k && (A[k] - A[j+1]) * (A[j+1] - A[i]) >= (A[k] - A[j]) * (A[j] - A[i])) {
					j++;
				}
				ans += (A[k] - A[j]) * (A[j] - A[i]);
			}
		}
		cout << ans << '\n';
	}

	return 0;
}
