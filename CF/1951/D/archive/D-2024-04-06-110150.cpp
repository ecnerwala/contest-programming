#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int64_t N, K; cin >> N >> K;
		if (K > N) {
			cout << "NO" << '\n';
		} else if (K <= (N+1)/2) {
			cout << "YES" << '\n';
			cout << 2 << '\n';
			cout << N - (K-1) << ' ' << 1 << '\n';
		} else if (K == N) {
			cout << "YES" << '\n';
			cout << 1 << '\n';
			cout << 1 << '\n';
		} else {
			cout << "NO" << '\n';
		}
	}

	return 0;
}

// Stall counts have to be decreasing
