#include<bits/stdc++.h>
using namespace std;

const int MAXN = 2e5;
int dp[MAXN];

void precomp() {
	for (int i = 0; i < MAXN; i++) dp[i] = i;
	for (int i = 1; i * i < MAXN; i++)  {
		for (int j = 0; j + i * i < MAXN; j++) {
			dp[j + i * i] = min(dp[j + i * i], dp[j] + 1);
		}
	}
	for (int  i = 0; i < MAXN; i++) {
		assert(dp[i] <= 4);
	}
}

int main() {
	ios_base::sync_with_stdio(0);
	precomp();

	int T; cin >> T;

	for(int case_num = 1; case_num <= T; case_num ++) {
		int N; cin >> N;
		cout << "Case #" << case_num << ": " << dp[N] << '\n';
	}

	return 0;
}
