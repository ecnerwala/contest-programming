#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);
	string S; cin >> S;
	int N = int(S.size());
	int64_t ans = 0;
	array<int, 26> cnt; cnt.fill(0);
	for (int i = N-1; i >= 0; i--) {
		cnt[S[i] - 'a']++;
		if (i+1 < N && S[i] == S[i+1]) {
			// then make everything into it
			for (int c = 0; c < 26; c++) {
				if (c == S[i] - 'a') continue;
				ans += cnt[c];
				cnt[S[i] - 'a'] += cnt[c];
				cnt[c] = 0;
			}
		}
	}
	cout << ans << '\n';
	return 0;
}
