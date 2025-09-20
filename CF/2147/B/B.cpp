#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N; cin >> N;
		for (int i = N-1; i >= 1; i--) {
			cout << i << ' ';
		}
		cout << N << ' ';
		for (int i = 1; i <= N-1; i++) {
			cout << i << ' ';
		}
		cout << N << '\n';
	}

	return 0;
}
