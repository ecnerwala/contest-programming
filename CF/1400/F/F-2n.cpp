#include<bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	string S; cin >> S;
	//string S = string(1000, '1');
	int N = int(S.size());
	int X; cin >> X;
	//int X = 20;
	vector<int> dp(1 << (X-1), N+1);
	dp[0] = 0;
	vector<int> ndp(1 << (X-1));

	int factor_mask = 0;
	for (int v = 1; v < X; v++) {
		if (X % v == 0) {
			factor_mask |= 1 << (v-1);
		}
	}

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
