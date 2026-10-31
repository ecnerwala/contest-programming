#include<bits/stdc++.h>
using namespace std;

using ll = long long;

const int MAXN = 1.1e5;
int N; ll M;
ll V[MAXN * 2];
ll pref[MAXN * 2];
int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	cin >> N >> M;
	for (int i = 0; i < N; i++) cin >> V[i];
	sort(V, V + N);

	for (int i = 0; i < N; i++) {
		V[i+N] = V[i] + M;
	}

	pref[0] = 0;
	for (int i = 0; i < 2 * N; i++) {
		pref[i+1] = pref[i] + V[i];
	}

	ll ans = 1e18;
	for (int s = 0; s < N; s++) {
		// median of s..s+N
		int m = s + N/2;
		int e = s + N;
		ll cnd = ((m - s) * V[m] - (pref[m] - pref[s])) + ((pref[e] - pref[m]) - (e - m) * V[m]);
		ans = min(ans, cnd);
	}
	cout << ans << '\n';

	return 0;
}
