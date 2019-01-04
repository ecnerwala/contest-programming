#include<bits/stdc++.h>
using namespace std;

const int MAXN = int(1e6) + 200;
int N;
int P[MAXN];

int dp[MAXN];

int main() {
	ios_base::sync_with_stdio(0);
	string S;
	getline(cin, S);
	for (string::iterator it = S.begin(); it != S.end(); it++) {
		N =	10 * N + (*it - '0');
	}
	getline(cin, S);
	S += ' ';
	for (int i = 2, j = 0; i <= N; i++) {
		while (S[j] != ' ') {
			P[i] = 10 * P[i] + (S[j] - '0');
			j++;
		}
		j++;
	}
	int res = 0;
	for (int i = N; i > 0; i--) {
		if (dp[i] == 0) {
			dp[i] ++;
		} else {
			dp[i] --;
			res++;
		}

		dp[P[i]] += dp[i];
	}

	printf("%d\n", res);

	return 0;
}
