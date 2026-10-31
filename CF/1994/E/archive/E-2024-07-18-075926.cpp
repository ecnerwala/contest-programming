#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int K; cin >> K;
		int a = 0;
		int b = 0;
		for (int k = 0; k < K; k++) {
			int N; cin >> N;
			for (int i = 1; i < N; i++) { int p; cin >> p; }
			b |= (a & N);
			a |= N;
		}
		// b is everything with at least 2 bits
		if (b) {
			a |= (1 << (31 - __builtin_clz(b))) - 1;
		}
		cout << a << '\n';
	}

	return 0;
}
