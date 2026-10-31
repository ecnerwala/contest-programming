#include<bits/stdc++.h>
using namespace std;

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	int H, W, K; cin >> H >> W >> K;
	vector<string> S(H);
	for (int i = 0; i < H; i++) {
		cin >> S[i];
	}

	int ans = H-1 + W-1;
	for (int m = 0; m < (1<<(H-1)); m++) {
		int cnd = __builtin_popcount(m);
		vector<int> cnt(H);
		for (int cur = 0; cur < W; cur++) {
			for (int i = 0, j = 0; i < H; i++) {
				cnt[j] += S[i][cur] == '1';
				if (m & (1<<i)) j = i+1;
			}
			bool colGood = true;
			for (int i = 0; i < H; i++) {
				if (cnt[i] > K) colGood = false;
			}
			if (colGood) {
				continue;
			}
			cnd++;
			for (int i = 0; i < H; i++) cnt[i] = 0;

			for (int i = 0, j = 0; i < H; i++) {
				cnt[j] += S[i][cur] == '1';
				if (m & (1<<i)) j = i+1;
			}
			colGood = true;
			for (int i = 0; i < H; i++) {
				if (cnt[i] > K) colGood = false;
			}
			if (colGood) {
				continue;
			}

			cnd += H-1 + W-1;
		}
		ans = min(ans, cnd);
	}
	cout << ans << '\n';

	return 0;
}
