#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N; cin >> N;
		std::vector<int> A(N);
		for (auto& a : A) {
			cin >> a;
		}
		std::sort(A.begin(), A.end());
		int v = A[1] - A[0];
		int ans = A[1] - A[0] + A[3] - A[2];
		for (int i = 2; i+1 < N; i++) {
			v = std::min(v, A[i-1] - A[i-2]);
			ans = std::min(ans, v + (A[i+1] - A[i]));
		}
		cout << ans << '\n';
	}

	return 0;
}
