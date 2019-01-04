#include<iostream>

using namespace std;

typedef long long ll;

const int MAXN = 6e5;
int N;
char S[MAXN];

int dp[MAXN];

int main() {
	ios_base::sync_with_stdio(0);
	cin >> N >> S;

	ll res = 0;

	dp[0] = 0;
	int maxR = 0;
	int cent = 0;
	for(int i = 1; i < N; i++) {
		if(i >= maxR) {
			dp[i] = 0;
			while(dp[i] < min(i, N - i) && S[i + dp[i]] != S[i - dp[i] - 1]) {
				dp[i] ++;
			}
			maxR = i + dp[i];
			cent = i;
		} else {
			dp[i] = dp[cent * 2 - i];
			int r = i + dp[i];
			if(r > maxR) {
				dp[i] = maxR - i;
			} else if(r == maxR) {
				while(dp[i] < min(i, N - i) && S[i + dp[i]] != S[i - dp[i] - 1]) {
					dp[i] ++;
				}
				maxR = i + dp[i];
				cent = i;
			}
		}
		//cerr << dp[i] << '\n';
	}

	for(int i = 0; i < N; i++) res += dp[i];
	cout << res << '\n';
	return 0;
}
