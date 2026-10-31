#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N; cin >> N;
		std::vector<int> P(N);
		for (auto& p : P) cin >> p;

		int ans = 0;
		int best = N+1;
		for (int i = N-1; i >= 0; i--) {
			int cnd = P[i];
			if (cnd < best) {
				best = cnd;
				ans++;
			}
		}
		cout << ans << '\n';
	}

	return 0;
}
