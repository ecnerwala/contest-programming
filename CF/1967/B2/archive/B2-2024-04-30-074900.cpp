#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	// b' * g is multiple of (a' + b'), so g is a multiple of a' + b'
	// (a' + b') * b' * k
	// (a' + b') * a' * k
	int T; cin >> T;
	while (T--) {
		int N, M; cin >> N >> M;
		int64_t ans = 0;
		for (int a = 1; a <= N; a++) {
			for (int b = 1; (a + b) * a <= N && (a + b) * b <= M; b++) {
				if (std::gcd(a, b) == 1) {
					ans += std::min(N / a / (a+b), M / b / (a+b));
				}
			}
		}
		cout << ans << '\n';
	}

	return 0;
}
