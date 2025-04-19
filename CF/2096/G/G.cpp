#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N; cin >> N;
		int Q = 1;
		int ways = 1;
		while (ways < N) {
			Q++;
			ways *= 3;
		}
		assert(ways >= N);
		std::vector<std::vector<int>> A(Q);
		std::vector<std::vector<int>> B(Q);
		std::vector<std::vector<int>> C(Q);
		for (int q = 0; q < Q; q++) {
			A[q].reserve(N/2);
			B[q].reserve(N/2);
			C[q].reserve(N);
		}
		for (int i = 0; i < N/2; i++) {
			// use balanced ternary
			int v = i+1;
			int tot = 0;
			for (int q = 0; q < Q; q++) {
				int d = (q == Q-1 ? -tot : v) % 3;
				if (d >= 2) d -= 3;
				if (d <= -2) d += 3;
				assert(-1 <= d && d <= +1);
				if (d == +1) {
					A[q].push_back(2*i+0);
					B[q].push_back(2*i+1);
				} else if (d == -1) {
					B[q].push_back(2*i+0);
					A[q].push_back(2*i+1);
				} else if (d == 0) {
					C[q].push_back(2*i+0);
					C[q].push_back(2*i+1);
				} else assert(false);
				tot += d;
				if (q < Q-1) {
					v -= d;
					assert(v % 3 == 0);
					v /= 3;
				}
			}
			assert(tot % 3 == 0);
			assert(v == 0);
		}

		if (N & 1) {
			// N-1 goes in neither, noop is fine
			for (int q = 0; q < Q; q++) {
				C[q].push_back(N-1);
			}
		}
		cout << Q << '\n';
		for (int q = 0; q < Q; q++) {
		}
	}

	return 0;
}
