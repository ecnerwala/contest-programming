#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N, M, K; cin >> N >> M >> K;
		cout << std::min(N, K) * std::min(M, K) << '\n';
	}

	return 0;
}
