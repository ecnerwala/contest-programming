#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N, M, K; cin >> N >> M >> K;
		std::vector<int> vals(K+1);
		for (int m = 0; m < M; m++) {
			int r, c, a; cin >> r >> c >> a;
			r %= (K+1);
			a %= (K+1);
			vals[r] ^= a;
		}
		if (0 == *std::max_element(vals.begin(), vals.end())) {
			cout << "Kamu" << '\n';
		} else {
			cout << "Anda" << '\n';
		}
	}

	return 0;
}
