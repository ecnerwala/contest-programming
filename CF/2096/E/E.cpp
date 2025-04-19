#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N; cin >> N;
		// (total inversions + number of swaps) / 2
		std::string S; cin >> S;
		std::reverse(S.begin(), S.end());
		int cnt_B = 0;
		int cnt_P_1 = 0;
		int64_t ans = 0;
		for (int i = 0; i < N; i++) {
			if (S[i] == 'P') {
				ans += cnt_B;
				cnt_P_1 += (i & 1);
			} else if (S[i] == 'B') {
				cnt_B++;
			} else assert(false);
		}
		ans += std::abs(cnt_P_1 - (N - cnt_B) / 2);
		cout << ans / 2 << '\n';
	}

	return 0;
}
