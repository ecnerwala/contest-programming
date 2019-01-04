#include<iostream>

using namespace std;

typedef long long ll;

const int MAXN = 620;
const int MAXK = 100;

int N;
int K;
ll cnt[MAXN][MAXN];
ll pref[MAXN][MAXN];

ll dp[MAXK][MAXN]; // max # tickets with k checks, the last of which happened just before station i
int prv[MAXK][MAXN]; // previous i

int res[MAXK];

int main() {
	ios_base::sync_with_stdio(0);
	cin >> N >> K;
	for(int i = 0; i < N; i++) {
		for(int j = i + 1; j < N; j++) {
			cin >> cnt[i][j];
		}
	}

	for(int k = 0; k <= K; k++) {
		for(int j = 0; j < N; j++) {
			dp[k][j] = 0;
			prv[k][j] = 0;
		}
	}

	for(int i = 0; i <= N; i++) {
		pref[i][0] = pref[0][i] = 0;
	}

	for(int i = 0; i < N; i++) {
		for(int j = 0; j < N; j++) {
			pref[i + 1][j + 1] = pref[i + 1][j] + pref[i][j + 1] - pref[i][j] + cnt[i][j];
		}
	}

	for(int i = 0; i < N; i++) {
		for(int j = i + 1; j < N; j++) {
			//int val = 0;
			//val = # who j > board >= i, leave >= j
			//for(int a = i; a < j; a++) {
				//for(int b = j; b < N; b++) {
					//val += cnt[a][b];
				//}
			//}
			
			ll val = pref[j][N] - pref[i][N] - pref[j][j] + pref[i][j];

			for(int k = 0; k < K; k++) {
				ll cnd = dp[k][i] + val;
				if(cnd > dp[k + 1][j]) {
					dp[k + 1][j] = cnd;
					prv[k + 1][j] = i;
				}
			}
		}
	}

	int cur = 0;
	for(int i = 1; i < N; i++) {
		if(dp[K][i] > dp[K][cur]) cur = i;
	}

	for(int i = K; i > 0; i--) {
		res[i] = cur;
		cur = prv[i][cur];
	}

	for(int i = 1; i <= K; i++) {
		cout << res[i] << ((i == K) ? '\n' : ' ');
	}

	return 0;
}
