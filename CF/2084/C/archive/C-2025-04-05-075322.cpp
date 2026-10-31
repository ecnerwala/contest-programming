#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N; cin >> N;
		std::vector<int> A(N);
		std::vector<int> B(N);
		for (auto& a : A) { cin >> a; a--; }
		for (auto& b : B) { cin >> b; b--; }
		std::vector<int> aToB(N);
		for (int i = 0; i < N; i++) { aToB[A[i]] = B[i]; }
		bool good = true;
		int cnt_1 = 0;
		for (int i = 0; i < N; i++) {
			if (aToB[i] == i) {
				cnt_1++;
			} else if (aToB[aToB[i]] != i) {
				good = false;
			}
		}
		if (cnt_1 > 1) {
			good = false;
		}
		if (good) {
			std::vector<int> invA(N);
			for (int i = 0; i < N; i++) invA[A[i]] = i;
			std::vector<int> P(N, -1);
			int cur = 0;
			for (int i = 0; i < N; i++) {
				if (P[i] != -1) continue;
				if (A[i] == B[i]) {
					assert(N & 1);
					P[i] = N/2;
				} else {
					assert(B[invA[B[i]]] == A[i]);
					P[i] = cur;
					assert(P[invA[B[i]]] == -1);
					P[invA[B[i]]] = N-1-cur;
					cur++;
				}
			}
			assert(cur == N/2);
			std::vector<std::array<int, 2>> ops; ops.reserve(N-1);
			for (int i = 0; i < N; i++) {
				while (P[i] != i) {
					ops.push_back({i, P[i]});
					std::swap(P[i], P[P[i]]);
				}
			}
			cout << ops.size() << '\n';
			for (auto [a, b] : ops) { cout << a+1 << ' ' << b+1 << '\n'; }
		} else {
			cout << -1 << '\n';
		}
	}

	return 0;
}
