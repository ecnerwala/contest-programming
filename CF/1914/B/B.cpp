#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N, K; cin >> N >> K;
		for (int i = 1; i <= K; i++) cout << i << ' ';
		for (int i = N; i > K; i--) cout << i << " \n"[i-1==K];
	}

	return 0;
}
