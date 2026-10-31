#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N; cin >> N;
	std::vector<int> A(N);
	for (auto& a : A) cin >> a;

	using bs = bitset<int(2e5) + 1>;
	bs can_reach;
	can_reach[1] = true;
	int64_t ans = 0;
	int64_t cur = 1;
	for (int i = 0; i <= 2*N; i++) {
		if (can_reach[i]) {
			ans = std::max(ans, cur);
		}
		cur += (i < N ? A[i] : 0) - 1;
		can_reach[i] = false;
		if (i < N) {
			can_reach |= can_reach << A[i];
		}
	}
	cout << ans << '\n';

	return 0;
}
