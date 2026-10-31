#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N, K; cin >> N >> K;
		assert(2 <= K && K <= 5);
		if (K == 4) {
			int ans = 3;
			int num_1 = 0;
			int num_2 = 0;
			for (int i = 0; i < N; i++) {
				int a; cin >> a;
				a = (-a) & 3;
				ans = std::min(ans, a);
				num_2 += a == 2;
				num_1 += a == 3;
				if (a == 3) num_1++;
			}
			if (ans == 0 || num_2 >= 2) cout << 0 << '\n';
			else if (ans == 1 || (num_1 && num_2)) cout << 1 << '\n';
			else if (ans == 2 || num_1 >= 2) cout << 2 << '\n';
			else cout << 3 << '\n';
		} else {
			int ans = K-1;
			for (int i = 0; i < N; i++) {
				int a; cin >> a;
				a = K - 1 - (a-1)%K;
				ans = std::min(ans, a);
			}
			cout << ans << '\n';
		}
	}

	return 0;
}
