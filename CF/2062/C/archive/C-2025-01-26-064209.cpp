#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N; cin >> N;
		std::vector<int64_t> A(N);
		for (auto& a : A) cin >> a;
		int64_t ans = std::accumulate(A.begin(), A.end(), int64_t(0));
		while (int(A.size()) > 1) {
			for (int i = 0; i+1 < int(A.size()); i++) {
				A[i] = A[i+1] - A[i];
			}
			A.pop_back();
			ans = std::max(ans, std::abs(std::accumulate(A.begin(), A.end(), int64_t(0))));
		}
		cout << ans << '\n';
	}

	return 0;
}
