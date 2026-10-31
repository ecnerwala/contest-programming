#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N, K; cin >> N >> K;
		for (int i = 0; i < N; i++) {
			cout << ((i & 1) ? (i/2)+1 : N-(i/2)) << " \n"[i+1==N];
		}
	}

	return 0;
}
