#include<bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N; cin >> N;
		int M = N;
		vector<int> cnt(2*M+1, 0);
		while (N--) {
			int w; cin >> w; cnt[w]++;
		}
		int ans = 0;
		for (int s = 0; s <= 2*M; s++) {
			int cnd = 0;
			for (int i = 0; i < s - i; i++) {
				cnd += min(cnt[i], cnt[s-i]);
			}
			if (s % 2 == 0) cnd += cnt[s/2] / 2;
			ans = max(ans, cnd);
		}
		cout << ans << '\n';
	}

	return 0;
}
