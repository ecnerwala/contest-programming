#include<bits/stdc++.h>
using namespace std;

using ll = long long;
ll p10[19];

void precomp() {
	p10[0] = 1; for (int i = 1; i <= 18; i++) { p10[i] = p10[i-1]*10; }
}

// these may have leading digit 0
ll dfs(int l, ll d) {
	assert(l >= 0);
	d = abs(d);
	if (l == 1) {
		return d == 0 ? 10 : 0;
	}
	if (l == 0) {
		return d == 0 ? 1 : 0;
	}
	assert(l >= 2);

	ll ans = 0;
	for (int delta = -9; delta <= 9; delta++) {
		ll nd = d - delta + p10[l-1] * delta;
		if (nd % 10) continue;
		ans += dfs(l-2, nd / 10) * (10 - abs(delta));
	}
	return ans;
}

ll dfsNonZero(int l, ll d) {
	assert(l > 0);
	if (l == 1) {
		return d == 0 ? 9 : 0;
	}
	assert(l >= 2);
	ll ans = dfs(l, d);
	for (int dig = 0; dig <= 9; dig++) {
		ll nd = d + dig - p10[l-1] * dig;
		if (nd % 10) continue;
		ans -= dfs(l-2, nd / 10);
	}
	return ans;
}

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	precomp();

	int D; cin >> D;
	ll ans = 0;
	for (int L = 1; L <= 19; L++) {
		ans += dfsNonZero(L, D);
	}
	cout << ans << '\n';

	return 0;
}
