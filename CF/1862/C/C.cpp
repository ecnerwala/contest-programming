#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N; cin >> N;
		std::vector<int> A(N);
		for (auto& a : A) std::cin >> a;
		std::cout << ([&]() -> bool {
			if (A[0] != N) {
				return false;
			}
			std::vector<int> B(N);
			for (int i = 0; i < N; i++) {
				B[A[i] - 1] ++;
			}
			for (int i = N-1; i > 0; i--) {
				B[i-1] += B[i];
			}
			return A == B;
		}() ? "YES" : "NO") << '\n';
	}

	return 0;
}
