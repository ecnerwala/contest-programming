#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N; cin >> N;
		std::vector<int> A(N);
		for (auto& a : A) { cin >> a; a--; }
		std::vector<int> B(N);
		for (auto& b : B) { cin >> b; b--; }
		std::vector<std::pair<int, int>> ops;
		ops.reserve(N * (N-1) / 2);
		bool ans = [&]() -> bool {
			for (int i = 0; i < N; i++) {
				if (B[i] > A[i]) {
					return false;
				}
				for (int j = i+1; j < N; j++) {
					if (A[i] > A[j] && A[j] >= B[i]) {
						ops.push_back({i, j});
						std::swap(A[i], A[j]);
					}
				}
				if (A[i] != B[i]) return false;
			}
			return true;
		}();
		if (ans) {
			cout << ops.size() << '\n';
			for (auto [x, y] : ops) {
				cout << x+1 << ' ' << y+1 << '\n';
			}
		} else {
			cout << -1 << '\n';
		}
	}

	return 0;
}
