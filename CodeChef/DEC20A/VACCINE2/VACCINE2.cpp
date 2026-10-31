#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N, D; cin >> N >> D;
		int cnt = 0;
		for (int i = 0; i < N; i++) {
			int a; cin >> a;
			cnt += (a >= 80 || a <= 9);
		}
		cout << (cnt + D - 1) / D + (N - cnt + D - 1) / D << '\n';
	}

	return 0;
}
