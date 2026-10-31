#include<bits/stdc++.h>
using namespace std;
using ll = long long;
const int MOD = int(1e9)+7;

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	int N, M; cin >> N >> M;
	ll ans = 1;
	for (int i = 1; i <= N; i++) ans *= i, ans %= MOD;
	for (int i = 1; i <= M; i++) ans *= i, ans %= MOD;
	if (N == M) {
		ans *= 2;
	} else if (abs(N - M) == 1) {
		ans *= 1;
	} else {
		ans *= 0;
	}
	ans %= MOD;
	cout << ans << '\n';

	return 0;
}
