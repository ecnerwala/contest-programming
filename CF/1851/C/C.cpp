#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N, K; cin >> N >> K;
		std::vector<int> C(N);
		for (auto& c : C) cin >> c;
		int i = 0;
		for (int cnt = K; i < N; i++) {
			cnt -= (C[i] == C[0]);
			if (cnt == 0) break;
		}
		int j = N-1;
		for (int cnt = K; j >= 0; j--) {
			cnt -= (C[j] == C[N-1]);
			if (cnt == 0) break;
		}
		if ((i < j) || (i < N && C[0] == C[N-1])) {
			cout << "YES" << '\n';
		} else {
			cout << "NO" << '\n';
		}
	}

	return 0;
}
