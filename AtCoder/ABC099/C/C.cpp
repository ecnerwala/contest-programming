#include<bits/stdc++.h>
using namespace std;

const int MAXN = 2e5;
int N;
int dp[MAXN];

void update(int a) {
	for (int i = a; i <= N; i++) {
		dp[i] = min(dp[i], dp[i-a] + 1);
	}
}

int main() {
	cin >> N;
	for (int i = 0; i <= N; i++) {
		dp[i] = i;
	}
	for (int z = 1; z <= N; z *= 6) {
		update(z);
	}
	for (int z = 1; z <= N; z *= 9) {
		update(z);
	}
	cout << dp[N] << '\n';

	return 0;
}
