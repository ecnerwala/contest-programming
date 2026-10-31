#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N; cin >> N;
		std::vector<int64_t> A(N+1);
		for (int i = 1; i <= N; i++) cin >> A[i];
		bool good = true;
		for (int i = 1; i <= N; i++) {
			for (int j = 1; j <= N; j++) {
				if ((j % i == 0) != (A[j] % A[i] == 0)) {
					good = false;
				}
			}
		}
		std::vector<bool> is_prime(N+1, true);
		// we want all prime powers
		for (int p = 2; p <= N; p++) {
			if (!is_prime[p]) continue;
			for (int j = p+p; j <= N; j += p) is_prime[j] = false;

			for (int q = 1; q <= N / p; ) {
				q *= p;
				cerr << "checking " << q << '\n';
				int64_t g = A[q];
				for (int i = 1; i <= N; i++) {
					if (i % q == 0) continue;
					g = std::gcd(g, A[q] / std::gcd(A[i], A[q]));
				}
				if (g == 1) {
					good = false;
				}
			}
		}
		cout << (good ? "Yes" : "No") << '\n';
	}

	return 0;
}
