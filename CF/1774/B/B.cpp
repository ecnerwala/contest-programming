#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N, M, K; cin >> N >> M >> K;
		bool is_bad = false;
		int num_big = 0;
		for (int i = 0; i < M; i++) {
			int v; cin >> v;
			if (v == (N / K) + 1) {
				num_big++;
			} else if (v > (N / K) + 1) {
				is_bad = true;
			}
		}
		cout << (!is_bad && num_big <= N % K ? "YES" : "NO") << '\n';
	}

	return 0;
}
