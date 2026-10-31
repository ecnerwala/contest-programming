#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N, M, K; cin >> N >> M >> K;
		assert(M * K < N);

		std::vector<int> A(N);
		int cur = 0;
		for (int i = 0; i < K; i++) {
			int m = (N - 1 - i) % K;
			for (int v = m; v < N; v += K) {
				A[v] = cur / (M+1);
				cur++;
			}
		}
		for (int i = 0; i < N; i++) {
			cout << A[i] << " \n"[i+1==N];
		}
	}

	return 0;
}
