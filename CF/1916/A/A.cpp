#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N, K; cin >> N >> K;
		std::vector<int> B(N); for (auto& b : B) cin >> b;
		int r = 2023;
		for (int b : B) {
			if (r % b) { r = 0; break; }
			else r /= b;
		}
		if (r == 0) {
			cout << "NO" << '\n';
		} else {
			cout << "YES" << '\n';
			cout << r;
			for (int i = 0; i < K-1; i++) cout << ' ' << 1;
			cout << '\n';
		}
	}

	return 0;
}
