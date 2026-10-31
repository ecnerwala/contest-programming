#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int64_t A, B; cin >> A >> B;
		// both odd or both even
		if (A % 2 == 0 || B % 2 == 0) {
			if (B % 2 == 0 && A * B % 4 == 0) {
				cout << (A * B / 2 + 2) << '\n';
			} else {
				cout << -1 << '\n';
			}
		} else {
			cout << (A * B + 1) << '\n';
		}
	}

	return 0;
}
