#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N; cin >> N;
		std::vector<std::pair<int, int>> A(N);
		for (int i = 0; i < N; i++) {
			cin >> A[i].first; A[i].second = i;
		}
		std::sort(A.begin(), A.end());
		int ans = -1;
		for (int i = 0; i < N; i++) {
			if (A[i].second != i) {
				int d = std::max(A[i].first - A.front().first, A.back().first - A[i].first);
				if (ans == -1 || ans < d) ans = d;
			}
		}
		cout << ans << '\n';
	}

	return 0;
}
