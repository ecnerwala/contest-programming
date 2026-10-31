#include<bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	for (int case_num = 1; case_num <= T; case_num ++) {

		int N, K; cin >> N >> K;
		std::vector<int> A(N); for (auto& a : A) cin >> a;
		std::vector<int> B(N); for (auto& b : B) cin >> b;

		auto solve = [&]() -> bool {
			assert(N >= 2);
			if (N == 2) {
				return B[K%2] == A[0] && B[(K+1)%2] == A[1];
			}
			assert(N >= 3);
			if (K == 0) {
				return A == B;
			}

			std::vector<int> fail(N+1);
			fail[0] = -1;
			for (int i = 0; i < N; i++) {
				int cnd = fail[i];
				while (cnd >= 0 && A[i] != A[cnd]) {
					cnd = fail[cnd];
				}
				fail[i+1] = cnd+1;
			}

			int cur = 0;
			for (int i = 0; i < 2*N; i++) {
				int v = B[i%N];
				while (cur >= 0 && (cur == N || A[cur] != v)) {
					cur = fail[cur];
				}
				cur++;
				if (cur == N) {
					if (K == 1 && i % N == N-1) continue;
					return true;
				}
			}
			return false;
		};

		bool ans = solve();
		cout << "Case #" << case_num << ": " << (ans ? "YES" : "NO") << '\n';
	}

	return 0;
}
