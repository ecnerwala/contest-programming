#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N, K; cin >> N >> K;
		int ans = 0;
		while (N) {
			ans += N % K;
			N /= K;
		}
		cout << ans << '\n';
	}

	return 0;
}
