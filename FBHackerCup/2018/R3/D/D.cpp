#include<bits/stdc++.h>
using namespace std;

const int MAXN = 600;
int H[MAXN];

using pii = pair<int, int>;
const int MAXM = 60;
int M;
pii P[MAXM];

int O[MAXM]; // last guy this overlaps with

int dp[MAXM][MAXM];

int go() {
	sort(P, P + M);
	//for (int i = 0; i < M; i++) { cerr << P[i].first << ' ' << P[i].second << '\n'; }
	for (int i = 0; i < M; i++) {
		assert(P[i].first <= P[i].second);
		O[i] = i;
		while (O[i]+1 < M && P[i].second >= P[O[i]+1].first) {
			O[i] ++;
		}
	}
	//for (int i = 0; i < M; i++) { cerr << i << ' ' << O[i] << '\n'; }

	memset(dp, 0, sizeof(dp));

	// now our problem is much simpler: each person overlaps with some number of others
	for (int r = 0; r <= M-1; r++) {
		for (int l = r; l >= 0; l--) {
			for (int k = l; k <= r; k++) {
				int cnt = 0;
				for (int i = l; i <= k; i++) {
					cnt += (O[i] <= r && O[i] >= k);
				}
				//cerr << l << ' ' << k << ' ' << r << ' ' << cnt << '\n';
				int v = cnt * (cnt-1) / 2;
				if (k > l) v += dp[l][k-1];
				if (k < r) v += dp[k+1][r];
				dp[l][r] = max(dp[l][r], v);
			}
		}
	}

	return dp[0][M-1];
}

int main() {
	int num_cases; cin >> num_cases;
	for (int case_num = 1; case_num <= num_cases; case_num++) {
		int N;
		int W;
		cin >> N >> M >> W;
		for (int i = 0; i < N; i++) {
			cin >> H[i];
		}
		for (int i = 0; i < M; i++) {
			int p, j; cin >> p >> j; p--;
			//assert(0 <= p && p < N);
			int l = p, r = p;
			while (l-1 >= 0 && j > W-H[l]) {
				l--;
			}
			while (r+1 < N && j > W-H[r]) {
				r++;
			}
			assert(l <= r);
			assert(0 <= l);
			assert(r < N);
			P[i] = pii(l, r);
		}
		//cerr << "Case #" << case_num << ":" << '\n';
		int res = go();
		cout << "Case #" << case_num << ": " << res << '\n';
	}

	return 0;
}
