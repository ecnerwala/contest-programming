#include<bits/stdc++.h>
using namespace std;

using ll = long long;

const int MAXN = 20;
int N;
ll A[MAXN];

ll mem[MAXN * MAXN / 2][1 << 19];

ll solve(int i, int j, ll wi, ll wj, uint32_t state) {
	if (j - i <= 1) {
		return 0;
	}
	int ind = (j - 1) * (j - 2) / 2 + i;
	if (mem[ind][state] != -1) {
		//return mem[ind][state];
	}
	ll ans = 1e18;
	ll wk = wi + wj;
	for (int k = i+1; k <= j-1; k++) {
		ll cnd = wk * A[k];
		cnd += solve(i, k, wi, wk, state * 2 + 0);
		cnd += solve(k, j, wk, wj, state * 2 + 1);
		ans = min(ans, cnd);
	}
	return mem[ind][state] = ans;
}

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	cin >> N;
	for (int i = 0; i < N; i++) cin >> A[i];
	memset(mem, -1, sizeof(mem));
	ll ans = A[0] + A[N-1] + solve(0, N-1, 1, 1, 1);
	cout << ans << '\n';

	return 0;
}
