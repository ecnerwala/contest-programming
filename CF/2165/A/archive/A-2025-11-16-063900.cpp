#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N; cin >> N;
		std::vector<int> A(N); for (auto& a : A) cin >> a;
		std::rotate(A.begin(), std::max_element(A.begin(), A.end()), A.end());
		int64_t ans = 0;
		for (int i = 0; i+1 < N; i++) ans += max(A[i], A[i+1]);
		cout << ans << '\n';
	}

	return 0;
}
