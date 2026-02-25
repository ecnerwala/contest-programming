#include <bits/stdc++.h>

int main() {
	using namespace std;
	int T; cin >> T;
	while (T--) {
		int N, M; cin >> N >> M;
		std::vector<std::vector<int>> A(N, std::vector<int>(M));
		std::vector<int64_t> s(N+M-1);
		std::vector<int64_t> d(N+M-1);
		for (int i = 0; i < N; i++) {
			for (int j = 0; j < M; j++) {
				cin >> A[i][j];
				s[i+j] += A[i][j];
				d[i+M-1-j] += A[i][j];
			}
		}
		int64_t ans = 0;
		for (int i = 0; i < N; i++) {
			for (int j = 0; j < M; j++) {
				int64_t cnd = s[i+j] + d[i+M-1-j] - A[i][j];
				ans = std::max(cnd, ans);
			}
		}
		cout << ans << '\n';
	}
}
