#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N; cin >> N;
		std::vector<int> C(N);
		for (auto& c : C) {
			cin >> c;
		}
		int K; cin >> K;
		for (int i = N-1; i > 0; i--) {
			C[i-1] = std::min(C[i-1], C[i]);
			C[i] -= C[i-1];
		}
		int cur = K;
		for (int i = 0; i < N; i++) {
			if (C[i] > 0) {
				cur = std::min(cur, K / C[i]);
			}
			K -= cur * C[i];
			std::cout << cur << " \n"[i+1 == N];
		}
	}

	return 0;
}
