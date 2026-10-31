#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N; cin >> N;
		if (N % 2 == 0) {
			cout << 1 << ' ' << N/2 << '\n';
		} else {
			cout << -1 << '\n';
		}
	}

	return 0;
}
