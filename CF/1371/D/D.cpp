#include<bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);
	int T; cin >> T;
	while (T--) {
		int N, K; cin >> N >> K;
		cout << (K % N == 0 ? 0 : 2) << '\n';
		for (int i = 0; i < N; i++) {
			for (int j = 0; j < N; j++) {
				int d = (j - i + N) % N;
				if (d * N + i < K) {
					cout << '1';
				} else {
					cout << '0';
				}
			}
			cout << '\n';
		}
	}

	return 0;
}
