#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N; cin >> N;
		std::vector<int> A(N);
		for (auto& a : A) cin >> a;
		std::vector<int> B(N);
		for (auto& b : B) cin >> b;

		int RT = 0;
		while ((RT + 1) * (RT + 1) <= 2 * N) RT++;

		int64_t ans = 0;
		for (int v = 1; v <= RT; v++) {
			std::vector<int> freq(N+1);
			for (int i = 0; i < N; i++) {
				if (A[i] == v) freq[B[i]]++;
			}
			for (int i = 0; i < N; i++) {
				int bj = A[i] * v - B[i];
				if (1 <= bj && bj <= N) {
					ans += freq[bj] * (A[i] <= RT ? 1 : 2);
				}
			}
		}

		for (int i = 0; i < N; i++) {
			if (int64_t(A[i]) * int64_t(A[i]) == 2 * B[i]) {
				ans--;
			}
		}
		ans /= 2;
		cout << ans << '\n';
	}

	return 0;
}
