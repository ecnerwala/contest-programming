#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N; cin >> N;
		// we need: ends in 6, alternating sum == 0 mod 11
		if (N & 1) {
			if (N <= 3) {
				cout << -1 << '\n';
			} else {
				cout << std::string(N-5, '3') << "36366" << '\n';
			}
		} else {
			cout << std::string(N - 2, '3') << "66" << '\n';
		}
	}

	return 0;
}
