#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);
	int T; cin >> T;
	while (T--) {
		string S; cin >> S; int N = int(S.size());
		std::array<int, 26> cnt; cnt.fill(0);
		for (char c : S) cnt[c-'a']++;

		int ans = 0;
		for (int i = 0; i < 26; i++) ans += cnt[i]/2;
		ans = min(ans, N/3);
		cout << ans << '\n';
	}

	return 0;
}

// Greedy algorithm: pick 2 of the same character, and any other 3rd, and delete them
//
// aabbcd
// Optimal: aac, bbd
// Greedy: aab, -bcd-
