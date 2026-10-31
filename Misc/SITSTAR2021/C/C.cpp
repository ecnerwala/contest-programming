#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N, M, P; cin >> N >> M >> P;
		if (N > M) swap(N, M);
		assert(N <= M);
		bool ans = false;
		for (int i = 1; i * i <= P; i++) {
			if (P % i == 0 && i <= N && P/i <= M) {
				ans = true;
				break;
			}
		}
		cout << (ans ? "YES" : "NO") << '\n';
	}

	return 0;
}
