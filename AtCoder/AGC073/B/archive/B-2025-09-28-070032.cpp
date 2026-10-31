#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N; cin >> N;
		std::vector<int64_t> A(N);
		for (auto& a : A) cin >> a;
		int64_t mi = A[0], ma = A[0] + A[1] - std::gcd(A[0], A[1]);
		while (ma - mi > 1) {
			int64_t md = mi + (ma - mi) / 2;
			([&]() -> bool {
				// can we do it using numbers from 0..md inclusive?
				int64_t num_e = 0;
				int64_t g = A[0];
				for (int64_t a : A) {
					if (a > md) break;
					g = std::gcd(g, a);
					num_e += md - a + 1;
				}
				return num_e > (md+1) - g;
			}() ? ma : mi) = md;
		}
		cout << ma << '\n';
	}

	return 0;
}
