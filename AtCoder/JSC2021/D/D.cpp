#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int64_t N, P; cin >> N >> P;

	const int MOD = int(1e9)+7;
	int64_t ans = P-1; N--;
	P -= 2;
	while (N) {
		if (N & 1) ans *= P, ans %= MOD;
		P *= P; P %= MOD;
		N >>= 1;
	}
	cout << ans << '\n';

	return 0;
}
