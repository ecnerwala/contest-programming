#include<bits/stdc++.h>
using namespace std;

const int MAXN = int(1e6) + 200;
int N;
int P[MAXN];

int dp[MAXN];

int main() {
	scanf("%d", &N);
	P[0] = -1;
	for (int i = 1; i < N; i++) {
		scanf("%d", &P[i]);
		P[i] --;
	}
	int res = 0;
	for (int i = N-1; i >= 0; i--) {
		if (dp[i] == 0) {
			dp[i] += 1;
		} else {
			dp[i] -= 1;
			res++;
		}

		int p = P[i];
		if (p == -1) continue;
		dp[p] += dp[i];
	}

	printf("%d\n", res);

	return 0;
}
