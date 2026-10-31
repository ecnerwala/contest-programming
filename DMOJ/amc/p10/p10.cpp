#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);
	const int MOD = int(1e9)+7;
	int N; cin >> N;
	int64_t ans = 0;
	for (int i = -N; i <= N; i++) {
		int64_t a; cin >> a;
		a %= MOD;
		ans = (ans + (a * i) % MOD * i) % MOD;
	}
	ans += (ans & 1) ? MOD : 0;
	ans >>= 1;
	cout << ans << '\n';

	return 0;
}
