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
			if (K == 1) {
				// Check that there's a rotation that's not the identity
				if (A == B) return false;
			}

			// Otherwise, just check if they're rotations of each other
			auto get_canon = [&](std::vector<int>& V) {
				auto it = std::find(V.begin(), V.end(), 1);
				assert(it != V.end());
				std::rotate(V.begin(), it, V.end());
			};
			get_canon(A);
			get_canon(B);
			return A == B;
		};

		bool ans = solve();
		cout << "Case #" << case_num << ": " << (ans ? "YES" : "NO") << '\n';
	}

	return 0;
}
