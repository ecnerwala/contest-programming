#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N; cin >> N;
		std::vector<std::pair<int, int>> A(N);
		for (auto& [a, b] : A) {
			cin >> a >> b;
			a *= -1;
		}
		cout << ([&]() -> bool {
			if (N == 2) {
				return true;
			}
			std::sort(A.begin(), A.end());
			int tot_b = 0;
			for (int i = 0; i < N; i++) tot_b += A[i].second;
			for (int i = 0; i+1 < N; i++) {
				if (-A[i].first < -A[i+1].first + (tot_b - A[i+1].second)) {
					return false;
				}
			}
			return true;
		}() ? "Yes" : "No") << '\n';
	}

	return 0;
}
