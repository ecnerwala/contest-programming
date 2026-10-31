#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N; cin >> N;
		int L = 0, S = 1; while (S < N) S <<= 1, L++;
		vector<int> cnt(S);
		for (int i = 0; i < N; i++) {
			int a; cin >> a;
			cnt[a]++;
		}
		for (int l = 0; l < L; l++) {
			for (int i = 0; i < S; i++) {
				if (i & (1 << l)) continue;
				cnt[i] += cnt[i+(1<<l)];
			}
		}
		vector<int64_t> best(S);
		for (int i = 1; i < S; i++) {
			for (int l = 0; l < L; l++) {
				if (!(i & (1 << l))) continue;
				best[i] = max(best[i], best[i - (1 << l)] + (int64_t(cnt[i]) << l));
			}
		}
		cout << best[S-1] << '\n';
	}

	return 0;
}
