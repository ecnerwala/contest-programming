#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N; int64_t C; cin >> N >> C;
		std::vector<int64_t> A(N); for (auto& a : A) cin >> a;
		if (C >= 0) {
			std::sort(A.begin(), A.end());
			for (int i = 0; i < N; i++) {
				cout << A[i] << " \n"[i+1==N];
			}
			continue;
		}
		assert(C < 0);
		std::sort(A.begin(), A.end(), std::greater<>());
		for (int i = 0; i < N; i++) {
			for (int j = N-1; j > i; j--) {
				int64_t delta = 0;
				delta -= std::abs(A[j] - A[j-1] - C);
				if (j+1 < N) {
					delta -= std::abs(A[j+1] - A[j] - C);
					delta += std::abs(A[j+1] - A[j-1] - C);
				}

				if (i-1 >= 0) {
					delta -= std::abs(A[i] - A[i-1] - C);
					delta += std::abs(A[j] - A[i-1] - C);
				}
				delta += std::abs(A[i] - A[j] - C);

				assert(delta >= 0);
				if (delta == 0) {
					// take A[j]
					std::rotate(A.begin() + i, A.begin() + j, A.begin() + j+1);
					break;
				}
			}
		}
		for (int i = 0; i < N; i++) {
			cout << A[i] << " \n"[i+1==N];
		}
	}

	return 0;
}
