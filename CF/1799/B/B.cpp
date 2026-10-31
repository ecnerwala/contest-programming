#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N; cin >> N;
		std::vector<int> A(N);
		for (auto& a : A) cin >> a;

		if (*std::min_element(A.begin(), A.end()) == 1) {
			if (*max_element(A.begin(), A.end()) == 1) {
				cout << 0 << '\n';
			} else {
				cout << -1 << '\n';
			}
			continue;
		}
		std::vector<std::array<int, 2>> ops; ops.reserve(30*N);
		while (true) {
			int j = int(std::min_element(A.begin(), A.end()) - A.begin());
			bool all_eq = true;
			for (int i = 0; i < N; i++) {
				while (A[i] > A[j]) {
					ops.push_back({i, j});
					A[i] = (A[i] + A[j] - 1) / A[j];
				}
				if (A[i] < A[j]) all_eq = false;
			}
			if (all_eq) break;
		}
		cout << ops.size() << '\n';
		for (auto [i, j] : ops) {
			cout << i+1 << ' ' << j+1 << '\n';
		}
	}

	return 0;
}
