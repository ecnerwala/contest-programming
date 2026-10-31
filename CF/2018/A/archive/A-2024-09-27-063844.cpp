#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N; int64_t K; cin >> N >> K;
		std::vector<int64_t> A(N);
		for (auto& a : A) cin >> a;
		std::sort(A.begin(), A.end());
		cout << [&]() -> int64_t {
			for (int i = 1; i < N; i++) {
				int64_t delta = (A[i] - A[i-1]) * i;
				if (K >= delta) {
					K -= delta;
				} else {
					cout << A[i-1] + (K / i) << '\n';
				}
			}
			return (K / N) + A[N-1];
		}() << '\n';
	}

	return 0;
}
