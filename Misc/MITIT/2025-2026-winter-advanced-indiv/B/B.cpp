#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N; cin >> N;
		std::array<std::vector<int>, 2> X;
		X[0].resize(N);
		X[1].resize(N);
		for (int i = 0; i < N; i++) {
			cin >> X[0][i] >> X[1][i];
		}
		int ans = 0;
		for (int z = 0; z < 2; z++) {
			int mx = X[z][0];
			for (int i = 0; i < N; i++) {
				ans = std::max(ans, mx - X[z][i]);
				mx = std::max(mx, X[z][i]);
			}
		}
		cout << ans << '\n';
	}

	return 0;
}
