#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int64_t N, A, B, X, Y, Z; cin >> N >> A >> B >> X >> Y >> Z;
		Y = std::min(Y, X * A);
		Z = std::min(Z, X * B);

		int64_t fixed = 0;

		{
			int64_t g = std::gcd(A, B);
			A /= g, B /= g;
			fixed += (N % g) * X;
			N /= g;
			X *= g;
		}
		int64_t num_full = N / A / B;
		if (num_full > 4) {
			fixed += std::min(A * Z, B * Y) * (num_full - 4);
			N -= (num_full - 4) * A * B;
		}

		if (A > B) {
			std::swap(A, B);
			std::swap(Y, Z);
		}
		assert(A <= B);

		int64_t ans = X * N;
		// how many Bs do we use
		for (int64_t nb = 0; B * nb <= N; nb++) {
			int64_t r = (N - nb * B);
			int64_t na = r / A;
			int64_t n1 = r % A;
			ans = std::min(ans, n1 * X + na * Y + nb * Z);
		}
		ans += fixed;
		cout << ans << '\n';
	}

	return 0;
}
