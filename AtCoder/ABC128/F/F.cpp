#include<bits/stdc++.h>
using namespace std;

using ll = long long;

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	int N; cin >> N;
	vector<ll> S(N);
	for (auto& v : S) cin >> v;

	ll ans = 0;
	for (ll d = 1; d < N-1; d++) {
		ll cnd = 0;
		for (int i = 0; d * (i+1) < N-1; i++) {
			if ((N-1) % d == 0 && d * i >= (N-1) - d * i) {
				break;
			}
			cnd += S[d*i];
			cnd += S[(N-1) - d*i];
			ans = max(ans, cnd);
		}
	}
	cout << ans << '\n';

	return 0;
}
