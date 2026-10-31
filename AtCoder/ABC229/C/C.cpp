#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N; int W; cin >> N >> W;
	std::vector<std::pair<int, int>> A(N);
	for (int i = 0; i < N; i++) {
		cin >> A[i].first >> A[i].second;
	}
	std::sort(A.begin(), A.end(), std::greater<>());

	int64_t ans = 0;
	for (auto [a, b] : A) {
		int v = std::min(b, W);
		W -= v;
		ans += a * int64_t(v);
	}
	cout << ans << '\n';

	return 0;
}
