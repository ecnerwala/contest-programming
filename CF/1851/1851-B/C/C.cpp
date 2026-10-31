#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N, K; cin >> N >> K;
		std::vector<int> C(N);
		for (auto& c : C) cin >> c;

		auto solve = [&]() -> bool {
			if (C[0] == C[N-1]) {
				int cnt = 0;
				for (int i = 0; i < N; i++) {
					if (C[i] == C[0]) cnt++;
				}
				return cnt >= K;
			}

			int kth_front;
			for (int i = 0, cnt = 0; ; i++) {
				if (i == N) {
					kth_front = N;
					break;
				}
				if (C[i] == C[0]) {
					cnt++;
					if (cnt == K) {
						kth_front = i;
						break;
					}
				}
			}
			int kth_back;
			for (int i = N-1, cnt = 0; i >= 0; i--) {
				if (i < 0) {
					kth_back = -1;
					break;
				}
				if (C[i] == C[N-1]) {
					cnt++;
					if (cnt == K) {
						kth_back = i;
						break;
					}
				}
			};
			return (kth_front < kth_back);
		};
		cout << (solve() ? "YES" : "NO") << '\n';
	}

	return 0;
}
