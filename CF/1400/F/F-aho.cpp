#include<bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	string S; cin >> S;
	int N = int(S.size());
	int X; cin >> X;
	vector<int> dp(1 << (X-1), N+1);
	dp[0] = 0;
	vector<int> ndp(1 << (X-1));

	vector<char> is_good(1 << (X-1), false);
	for (int m = 0; m < (1 << (X-1)); m++) {
		for (int v = 1; v < X; v++) {
			if (X % v == 0) {
				if (m & (1 << (v-1))) is_good[m] = true;
				if (m & (1 << (X-1 - v))) is_good[m] = true;
				for (int i = 0; i+v < X-1; i++) {
					if ((m & (1 << i)) && (m & (1 << (i+v)))) {
						is_good[m] = true;
					}
				}
			}
		}
		if (!is_good[m]) {
			vector<int> inds;
			for (int i = 0; i < X-1; i++) {
				if (m & (1 << i)) inds.push_back(i+1);
			}
			inds.push_back(X-1);
			for (int z = 0; z+1 < int(inds.size()); z++) {
				inds[z+1] -= inds[z];
			}

			for (int i : inds) {
			}
		}
	}

	for (char c : S) {
		// skip the character
		for (int i = 0; i < (1 << (X-1)); i++) {
			ndp[i] = dp[i] + 1;
		}

		for (int m = 0; m < (1 << (X-1)); m++) {
			if (dp[m] > N) continue;
			int nm = (m*2+1) << (c - '1');
			if ((nm & (1 << (X-1))) && !is_good[nm & ((1 << (X-1)) - 1)]) {
				continue;
			}
			nm &= ((1 << (X-1)) - 1);
			ndp[nm] = min(ndp[nm], dp[m]);
		}

		swap(dp, ndp);
	}

	cout << *min_element(dp.begin(), dp.end()) << '\n';

	return 0;
}
