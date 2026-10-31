#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N; cin >> N;
		std::vector<int> X(N); for (auto& x : X) cin >> x;
		if (N == 1) {
			cout << 0 << '\n';
		} else {
			// the global max is attained on some open interval
			int v = 1;
			int r_mi = 0, r_ma = X[1] - X[0];
			for (int i = 2; i < N; i++) {
				int d = X[i] - X[i-1];
				r_ma = std::min(r_ma, d);
				if (r_ma <= r_mi) {
					v--;
					r_mi = 0, r_ma = d;
				}
				v++;
				std::tie(r_mi, r_ma) = std::make_pair(d - r_ma, d - r_mi);
			}
			cout << v << '\n';
		}
	}

	return 0;
}
