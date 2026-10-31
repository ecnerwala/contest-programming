#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int W, F; cin >> W >> F;
		int N; cin >> N;
		std::vector<int> S(N);

		using bs = std::bitset<int(1e6) + 20>;
		bs vals;
		vals[0] = true;
		int tot = 0;
		for (int i = 0; i < N; i++) {
			cin >> S[i];
			tot += S[i];
			vals |= vals << S[i];
		}
		int ans = tot + 1;
		for (int i = 0; i <= tot; i++) {
			if (vals[i]) {
				int cnd = std::max((i + W - 1) / W, (tot - i + F - 1) / F);
				ans = std::min(ans, cnd);
			}
		}
		cout << ans << '\n';
	}

	return 0;
}
