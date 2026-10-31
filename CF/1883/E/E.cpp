#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N; cin >> N;
		std::vector<int> A(N);
		for (auto& a : A) cin >> a;
		int64_t ans = 0;
		int cur = 0;
		for (int i = 1; i < N; i++) {
			int diff = 0;
			if (A[i-1] <= A[i]) {
				while (cur + diff > 0 && (A[i-1] << (-(diff-1))) <= A[i]) {
					diff--;
				}
			} else {
				while (A[i-1] > (A[i] << diff)) {
					diff++;
				}
			}
			cur += diff;
			ans += cur;
		}
		cout << ans << '\n';
	}

	return 0;
}
