#include <bits/stdc++.h>

int signum(int64_t v) { return (v > 0) - (v < 0); }

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N; int64_t K; cin >> N >> K;
		std::vector<int64_t> A(N);
		for (auto& a : A) { cin >> a; a -= K; }
		assert(K >= 1);
		int tgt_signum = signum(A[0]);
		bool good = true;
		int64_t g = 0;
		for (auto a : A) {
			if (signum(a) != tgt_signum) {
				good = false;
				break;
			}
			g = std::gcd(g, std::abs(a));
		}
		if (good) {
			int64_t ans = 0;
			if (tgt_signum != 0) {
				assert(g != 0);
				for (auto a : A) {
					assert(a != 0);
					ans += (std::abs(a) / g) - 1;
				}
			}
			cout << ans << '\n';
		} else {
			cout << -1 << '\n';
		}
	}

	return 0;
}
