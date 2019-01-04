#include<bits/stdc++.h>

using namespace std;


const int MAXN = 6000;
const int MAXL = MAXN;
int dp[MAXL];

int N;
char C[MAXN];

const int MOD = 1e9;

int main() {
	cin >> N;
	dp[0] = 1;
	for(int i = 0; i < N - 1; i++) {
		char c; cin >> c;
		// if c == '+', all the evens are 0
		for(int l = c != '+'; l <= i; l += 2) {
			dp[l] = 0;
		}
		int sum = 0;
		for(int l = i + 1; l > 0; l--) {
			sum += dp[l - 1];
			if(sum >= MOD) sum -= MOD;
			dp[l] = sum;
		}
		dp[0] = sum;
	}
	cout << dp[0] % MOD << '\n';
}
