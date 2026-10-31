#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N, M; cin >> N >> M;
	if (N == 0) {
		if (M == 1) {
			cout << 0 << '\n';
		} else {
			cout << -1 << '\n';
		}
	} else {
		if (N > 9 * M) {
			cout << -1 << '\n';
		} else {
			while (M--) {
				int v = min(9, N);
				N -= v;
				cout << v;
			}
			cout << '\n';
		}
	}

	return 0;
}
