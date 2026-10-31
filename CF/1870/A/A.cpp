#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N, K, X; cin >> N >> K >> X;
		if (X == K) X = K-1;
		if (X < K-1 || N < K) cout << -1 << '\n';
		else cout << (N-K) * X + K * (K-1) / 2 << '\n';
	}

	return 0;
}
