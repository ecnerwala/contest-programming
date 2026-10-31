#include<bits/stdc++.h>
using namespace std;

const int MAXN = 60;
const int MAXL = 110;
int N;
int L[MAXN];

bool dp[2][MAXN*MAXL];

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	cin >> N;
	for (int i = 0; i < N; i++) cin >> L[i];
	sort(L, L+N);

	int totL = 0;
	for (int i = 0; i < N; i++) totL += L[i];

	int ans = totL + 1;

	int prefL = 0;
	dp[0][0] = 1;
	for (int i = 0; i < N-1; i++) {
		memset(dp[!(i&1)], 0, sizeof(dp[0]));
		for (int l = 0; l <= prefL; l++) {
			if (!dp[(i&1)][l]) continue;
			dp[!(i&1)][l] = true;
			dp[!(i&1)][l+L[i]] = true;
			int l1 = l+L[i];
			int l2 = totL - l1;
			if (L[i] * 2 < l1 && L[N-1] * 2 < l2) {
				ans = min(ans, abs(l1-l2));
			}
		}

		prefL += L[i];
	}
	if (ans > totL) {
		cout << -1 << '\n';
	} else {
		cout << ans << '\n';
	}

	return 0;
}
