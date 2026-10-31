#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N, K; cin >> N >> K;
		std::vector<int> A(N);
		for (auto& a : A) { cin >> a; a--; }

		std::vector<int> mis(K+1, N);
		std::vector<int> mas(K+1, -1);
		for (int i = 0; i < N; i++) {
			mis[A[i]] = std::min(mis[A[i]], i);
			mas[A[i]] = std::max(mas[A[i]], i);
		}

		std::vector<int64_t> ans(K);
		for (int k = K-1; k >= 0; k--) {
			bool has = mis[k] <= mas[k];
			mis[k] = std::min(mis[k], mis[k+1]);
			mas[k] = std::max(mas[k], mas[k+1]);
			if (!has) {
				ans[k] = 0;
				continue;
			}
			ans[k] = (mas[k] - mis[k] + 1) * 2;
		}

		for (int k = 0; k < K; k++) {
			cout << ans[k] << " \n"[k+1 == K];
		}
	}

	return 0;
}
