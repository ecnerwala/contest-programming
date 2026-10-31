#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N; cin >> N;
		int wA = (N+1)/2 - 1;
		int wB = N/2 - 1;
		if (wA == 0 && wB == 0) {
			wA = 1;
			wB = 1;
		} else if (wA == 0 || wB == 0) {
			cout << "NO" << '\n';
			continue;
		}
		cout << "YES" << '\n';
		for (int i = 0; i < N; i++) {
			cout << ((i & 1) ? wA : -wB) << " \n"[i+1==N];
		}
	}

	return 0;
}
