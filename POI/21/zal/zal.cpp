#include<bits/stdc++.h>
using namespace std;

typedef long long ll;

const ll INF = 1e18;

const int MAXN = 2e6;

ll S;

int N;
ll T[MAXN];

ll dp[MAXN]; // time for first i

int main() {
	scanf("%d %lld", &N, &S);
	S--;
	for(int i = 0; i < N; i++) {
		scanf("%lld", &T[i]);
		if(i && T[i] <= T[i - 1]) T[i] = T[i - 1] + 1;
	}

	dp[0] = 0;

	for(int i = 1; i <= N; i++) {
		ll v = T[i - 1];
		v -= (i - 1);

		assert(v >= 0);

		int mi = 0;
		int ma = i;
		// we have v - (j + 1) >= dp[j] - 2 (j + 1) + i at mi 
		
		while(ma - mi > 1) {
			int md = (mi + ma) / 2;
			if(v - md >= dp[md]) {
				mi = md;
			} else {
				ma = md;
			}
		}

		dp[i] = max(dp[mi], v - mi);
		if(ma < i) {
			dp[i] = min(dp[i], max(dp[ma], v - ma));
		}

		dp[i] += 2 * S;

		assert(dp[i] >= dp[i - 1]);
	}

	ll res = dp[N] + 2 * N;
	printf("%lld\n", res);
	return 0;
}
/*
 * dp[i] = min value of max(dp[j] + (i - j - 1), v) + 2 * S + i - j - 1
 * dp[i] = i + 2 * S + min(max(dp[j] - j - 1 + i, v) - j - 1)
 * dp[i] = 2 * i + 2 * S - 2 + min(max(dp[j] - 2 * j (increasing), v - i + 1 - j (decreasing)))
 * dp[i] - 2i = 2(S - 1) + min(max(dp[j] - 2j, (v - (i + 1)) - j))
 *
 * dp[i] = 2
 *
 */
