#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);
	int T; cin >> T;
	while (T--) {
		int N; int M; cin >> N >> M;
		int l = 0;
		for (int z = 0; z < M; z++) {
			int v; cin >> v;
			l = gcd(l, v);
		}
		// find the next divisor of l smaller than N
		cout << (N - [&]() -> int {
			int i = 1;
			for (i = 1; i*i <= l; i++) {
				if (l % i == 0 && l / i <= N) return l/i;
			}
			for (i--; i >= 1; i--) {
				if (l % i == 0 && i <= N) return i;
			}
			return 1;
		}()) << '\n';
	}

	return 0;
}
