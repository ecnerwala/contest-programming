#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N, M; cin >> N >> M;
		std::vector<std::pair<int, int>> A(N); for (auto& [a, i] : A) cin >> a;
		for (int i = 0; i < N; i++) A[i].second = i;
		std::sort(A.begin(), A.end());
		if (2 * M > N) {
			cout << -1 << '\n';
		} else if (M >= 1) {
			cout << N-M << '\n';
			for (int i = 0; i < N-2*M; i++) {
				cout << A[i].second+1 << ' ' << A[i+1].second+1 << '\n';
			}
			for (int i = 0; i < M; i++) {
				cout << A[N-1-i].second+1 << ' ' << A[N-1-M-i].second+1 << '\n';
			}
		} else {
			assert(M == 0);
			int s = N-2;
			int r = A[N-1].first;
			while (s >= 0 && r > 0) {
				r -= A[s].first;
				s--;
			}
			if (r <= 0) {
				cout << N-1 << '\n';
				s++;
				assert(s < N-1);
				for (int i = 0; i < s; i++) {
					cout << A[i].second+1 << ' ' << A[i+1].second+1 << '\n';
				}
				for (int i = N-2; i >= s; i--) {
					cout << A[i].second+1 << ' ' << A[N-1].second+1 << '\n';
				}
			} else {
				cout << -1 << '\n';
			}
		}
	}

	return 0;
}
