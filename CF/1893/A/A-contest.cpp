#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N, K; cin >> N >> K;
		std::vector<int> B(N);
		for (auto& b : B) { cin >> b; b--; }
		assert(K > 0);
		std::vector<int> nxt(N);
		for (int i = 0; i < N; i++) {
			if (0 <= B[i] && B[i] < N) {
				nxt[i] = (i + N-1 - B[i]) % N;
			} else {
				nxt[i] = -1;
			}
		}
		// can we go k steps
		std::vector<int> vis(N, -1);
		int cur = N-1;
		for (int t = K; cur != -1 && t > 0; t--) {
			if (vis[cur] != -1) {
				// cycle means good
				break;
			} else {
				vis[cur] = t;
			}
			cur = nxt[cur];
		}
		cout << (cur != -1 ? "Yes" : "No") << '\n';
	}

	return 0;
}
