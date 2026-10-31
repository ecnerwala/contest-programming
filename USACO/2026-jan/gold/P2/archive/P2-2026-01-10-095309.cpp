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

		std::vector<int> bit(N, 0);

		int64_t ans = 0;

		// smallest guys go in the middle
		for (int i = 0, j = 0; i < N; i = j) {
			while (j < N && A[j].first == A[i].first) j++;
			for (int k = i; k < j; k++) {
				int p = A[k].second;
				int cnt = 0;
				for (int a = p; a > 0; a &= a-1) {
					cnt += bit[a-1];
				}
				ans += std::min(cnt, i-cnt);
			}
			for (int k = i; k < j; k++) {
				int p = A[k].second;
				for (int a = p; a < N; a |= a+1) {
					bit[a]++;
				}
			}
		}
		cout << ans << '\n';
	}

	return 0;
}
