#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N, K; cin >> N >> K;
		std::vector<std::array<int, 2>> A(N);
		for (auto& [a, b] : A) cin >> a;
		for (auto& [a, b] : A) cin >> b;

		for (auto& p : A) {
			if (p[0] > p[1]) std::swap(p[0], p[1]);
		}
		std::sort(A.begin(), A.end());
		int64_t tot_cost = 0;
		for (int i = 0; i < N; i++) {
			tot_cost += A[i][1] - A[i][0];
		}
		int64_t min_loss = -1;
		for (int i = 0; i+1 < N; i++) {
			int64_t cnd_loss;
			if (A[i+1][0] <= A[i][1]) {
				cnd_loss = 0;
			} else {
				cnd_loss = 2 * int64_t(A[i+1][0] - A[i][1]);
			}
			if (i == 0 || cnd_loss < min_loss) min_loss = cnd_loss;
		}
		cout << tot_cost + min_loss << '\n';
	}

	return 0;
}
