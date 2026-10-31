#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);
	int K; cin >> K;
	int64_t ans = 0;
	for (int i = 1; i <= K; i++) {
		for (int j = i; j <= K; j += i) {
			ans += K / j;
		}
	}
	cout << ans << '\n';

	return 0;
}
