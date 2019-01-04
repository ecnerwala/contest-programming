#include<bits/stdc++.h>
using namespace std;

const int MAXM = 120;
const int MAXK = 60;

int M;
int B[MAXM];
int K;

const int MAXS = MAXM * MAXK;

using pii = pair<int, int>;

int dp[MAXM]; // maximum value of given i is negative

int go() {
	int res = 0;
	int minE = 0;
	for (int i = 0; i < M; i+=2) {
		minE = max(minE, B[i/2]);
	}
	for (int E = minE; E <= M * K; E++) {
		memset(dp, 0, sizeof(dp));
		for (int i = 0; i <= M; i++) {
			dp[i] = 0;
			if (i % 2 == 0 && B[i/2] >= 0) {
				// not actually a valid endpoint
				continue;
			}
			int sum = 0;
			int cnt = 0;
			for (int j = i-1; true; j--) {
				// location of the last negative
				if (j % 2 == 0 && B[j/2] >= 0) {
					// not actually a valid endpoint
				} else {
					int last = (j == -1) ? 0 : dp[j];
					if (sum > E) {
						break;
					}
					int v = last + min(E, sum + cnt * K);
					res = max(res, v - E);
					if (i % 2 == 0) {
						assert(B[i/2] < 0);
						v += B[i/2];
					} else {
						v += -1;
					}
					v = max(v, 0);
					dp[i] = max(dp[i], v);
				}
				if (j == -1) break;
				// include j in the rest
				if (j % 2 == 0) {
					if (B[j/2] < 0) {
						// not a valid j
						break;
					}
					sum += B[j/2];
				} else {
					cnt ++;
				}
			}
		}
	}
	return res;
}

int main() {
	ios_base::sync_with_stdio(0);
	int num_cases; cin >> num_cases;
	for (int case_num = 1; case_num <= num_cases; case_num ++) {
		cin >> M >> K;
		for (int i = 0; i < M; i++) {
			cin >> B[i];
		}
		M = 2*M-1;
		int res = go();
		cout << "Case #" << case_num << ": " << res << '\n';
	}

	return 0;
}
