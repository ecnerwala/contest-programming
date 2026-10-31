#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N; cin >> N;
		int M; cin >> M; assert(M == 1);
		std::vector<int> A(N-1);
		for (auto& a : A) cin >> a;
		std::vector<int> B(N);
		for (auto& b : B) cin >> b;

		A.push_back(1);

		std::sort(A.begin(), A.end());
		std::sort(B.begin(), B.end());
		int j = 0;
		int ans = 0;
		for (int i = 0; i < N; i++) {
			while (j < N && A[i] >= B[j]) j++;
			j = std::max(j, i);
			ans = std::max(ans, j-i);
		}
		cout << ans << '\n';
	}

	return 0;
}
