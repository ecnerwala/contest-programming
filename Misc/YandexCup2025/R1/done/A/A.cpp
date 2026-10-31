#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N, M; cin >> N >> M;
		int S = N + M;
		int K = 1;
		while (K * K < S) {
			K++;
		}
		if (K * K == S && (K % 2 == 1 || N % 2 == 0)) {
			cout << "YES" << '\n';
		} else {
			cout << "NO" << '\n';
		}
	}

	return 0;
}
