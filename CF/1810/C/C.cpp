#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N; int64_t C, D; cin >> N >> C >> D;
		std::vector<int> A(N);
		for (auto& a : A) cin >> a;

		std::sort(A.begin(), A.end());
		A.resize(std::unique(A.begin(), A.end()) - A.begin());

		int64_t ans = N * C + D;
		for (int i = 0; i < int(A.size()); i++) {
			int l = A[i];
			int num_keep = i+1;
			ans = std::min(ans, (N - num_keep) * C + (l - num_keep) * D);
		}

		cout << ans << '\n';
	}

	return 0;
}
