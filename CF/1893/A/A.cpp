#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N, K; cin >> N >> K;
		std::vector<int> B(N);
		for (auto& b : B) { cin >> b; b--; }
		K = std::min(K, N);
		int cur = N-1;
		for (int t = 0; t < K; t++) {
			if (0 <= B[cur] && B[cur] < N) {
				cur = (cur + N - 1 - B[cur]) % N;
			} else {
				cur = -1;
				break;
			}
		}
		cout << (cur != -1 ? "Yes" : "No") << '\n';
	}

	return 0;
}
