#include<bits/stdc++.h>
using namespace std;

bool is_good(int a, int b, int c, int d) {
	return a < b && b < c && c < d && b - a <= 10 && c - b <= 10 && d - c <= 10;
}

bool is_good(int a, int b, int c) {
	return a < b && b < c && ((b - a <= 10 && c - b <= 20) || (b - a <= 20 && c - b <= 10));
}

bool is_good(int a, int b) {
	return a < b && (b - a <= 40);
	// oops, should be b - a <= 30
}

const int MAXN = 2e5;
int N;
int D[MAXN];

int dp[MAXN];

int main() {
	int __T; cin >> __T;
	for(int T = 1; T <= __T; T++) {
		cin >> N;
		for(int i = 0; i < N; i++) cin >> D[i];
		/*
		dp[0] = 0;
		for(int i = 1; i <= N; i++) {
			dp[i] = dp[i - 1];
			if(i >= 2 && is_good(D[i - 2], D[i - 1])) dp[i] = min(dp[i], dp[i - 2]);
			if(i >= 3 && is_good(D[i - 3], D[i - 2], D[i - 1])) dp[i] = min(dp[i], dp[i - 3]);
			if(i >= 4 && is_good(D[i - 4], D[i - 3], D[i - 2], D[i - 1])) dp[i] = min(dp[i], dp[i - 4]);
			dp[i] ++;
		}
		cout << "Case #" << T << ": " << (4 * dp[N] - N) << '\n';
		*/
		int res = 0;
		for(int i = 0; i < N; ) {
			res++;
			if(i + 4 <= N && is_good(D[i], D[i + 1], D[i + 2], D[i + 3])) {
				i += 4;
			} else if(i + 3 <= N && is_good(D[i], D[i + 1], D[i + 2])) {
				i += 3;
			} else if(i + 2 <= N && is_good(D[i], D[i + 1])) {
				i += 2;
			} else {
				i ++;
			}
		}
		cout << "Case #" << T << ": " << (4 * res - N) << '\n';
	}
	return 0;
}
