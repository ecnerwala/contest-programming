#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N, Q; cin >> N >> Q;
	std::vector<std::pair<int, int>> A(N);
	for (auto& a : A) cin >> a.first >> a.second;

	while (Q--) {
		int l, r; cin >> l >> r; l--;
		int d; cin >> d;

		int64_t ans = 2e18;
		for (int i = l; i < r; i++) {
			ans = std::min(ans, A[i].first * int64_t(d) + A[i].second);
		}
		cout << ans << '\n';
	}

	return 0;
}
