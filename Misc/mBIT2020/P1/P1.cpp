#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N; int64_t K; cin >> N >> K;
	vector<int64_t> A(N);
	for (int i = 0; i < N; i++) cin >> A[i];
	sort(A.begin(), A.end());

	int64_t ans = 0;
	for (int i = 0; i < N; i++) {
		int64_t cur = A[i];
		while (i+1 < N && A[i+1] - A[i] <= K) {
			cur += A[++i];
		}
		ans = max(ans, cur);
	}
	cout << ans << '\n';

	return 0;
}
