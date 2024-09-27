#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N; int64_t K; cin >> N >> K;
		K /= N;
		for (int i = 0; i < N; i++) {
			int64_t a; cin >> a;
			K = std::min(K, a);
		}
		cout << K << '\n';
	}

	return 0;
}
