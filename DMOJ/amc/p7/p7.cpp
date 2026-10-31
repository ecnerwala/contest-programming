#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int64_t N; cin >> N;
	int64_t ans = 0;
	for (int64_t x = 1; x * x <= N; x++) {
		ans += (N / x - x) * 2 + 1;
	}
	cout << ans << '\n';

	return 0;
}
