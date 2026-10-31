#include<bits/stdc++.h>
using namespace std;

const int MAXH = 400;
const int MAXS = 400;
int H, S;
int P[MAXH][MAXS];
const int MAXK = MAXH * MAXS;
int K;
int L[MAXK];

// max context switches is S+1
int minLoss[2][MAXS];
int dp[MAXS];

int bestFlips[MAXS];

vector<int> go() {
	cin >> S >> H >> K;
	for (int j = 0; j < S; j++) {
		string str; cin >> str;
		for (int i = 0; i < H; i++) {
			P[i][j] = str[i] - 'A';
		}
	}
	for (int k = 0; k < K; k++) cin >> L[k];

	// for (int i = 0; i < H; i++) { for (int j = 0; j < S; j++) { cerr << P[i][j]; } cerr << '\n'; }

	memset(minLoss, 0, sizeof(minLoss));

	for (int z = 0; z < 2; z++) {
		for (int i = 0; i < H; i++) {
			int maxDp = 1;
			dp[1] = 0;
			for (int j = 0; j < S; j++) {
				for (int v = maxDp; v >= 1; v--) {
					int oldVal = dp[v];
					if (P[i][j] == (z + v-1) % 2) {
						// nothing changes
						dp[v] = oldVal;
					} else {
						if (v+1 > maxDp) {
							maxDp = v+1;
							dp[v+1] = oldVal;
						} else {
							dp[v+1] = min(dp[v+1], oldVal);
						}
						dp[v] = oldVal + 1;
					}
				}
			}
			for (int v = 1; v < maxDp; v++) {
				dp[v+1] = min(dp[v], dp[v+1]);
			}
			assert(maxDp <= S+1);
			assert(dp[maxDp] == 0);
			/*
			cerr << i << ' ' << z << ' ';
			for (int v = 1; v <= S+1; v++) {
				cerr << (v <= maxDp ? dp[v] : 0) << ' ';
			}
			cerr << '\n';
			*/

			for (int v = 1; v <= S+1; v++) {
				minLoss[z][v] += (v <= maxDp ? dp[v] : 0);
			}
		}
	}

	for (int i = 1; i <= S+1; i++) {
		bestFlips[i] = min(minLoss[0][i], minLoss[1][i]);
		//cerr << i << ' ' << bestFlips[i] << ' ' << minLoss[0][i] << ' ' << minLoss[1][i] << '\n';
	}

	//bestFlips is reverse sorted

	for (int i = 1; i <= S+1; i++) {
		bestFlips[i] *= -1;
	}

	vector<int> ans(K);
	for (int k = 0; k < K; k++) {
		ans[k] = int(lower_bound(bestFlips + 1, bestFlips + S+1 + 1, -L[k]) - bestFlips);
	}
	return ans;
}

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);

	int T; cin >> T;
	for (int t = 1; t <= T; t++) {
		// cerr << "Case #" << t << ":" << '\n';
		vector<int> ans = go();
		cout << "Case #" << t << ":";
		for (int a : ans) {
			cout << " " << a;
		}
		cout << '\n';
	}

	return 0;
}
