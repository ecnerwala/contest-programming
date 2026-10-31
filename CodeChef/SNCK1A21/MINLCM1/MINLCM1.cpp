#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int64_t X, K; cin >> X >> K;
		cout << X * 2 << ' ' << X * K * (X * K - 1) << '\n';
	}

	return 0;
}
