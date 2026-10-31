#include<bits/stdc++.h>
using namespace std;

using ll = long long;

const ll INF = 2e18;

ll N, M, P;
ll tot, cnt;
bool canAfford(ll v) {
	//cerr << "Check " << v << '\n';
	tot = 0;
	cnt = 0;
	for (ll i = 1; i <= N && i <= M && i * i <= v; i++) {
		tot += i * i;
		cnt++;
		if (tot > INF) return false;
	}
	for (ll i = 1; (i <= N && i <= M) && i * (i+1) <= v; i++) {
		{
			__int128_t a = min(M, v / i) - i;
			__int128_t x = a * (i * (i+1) + i * (i + a)) / 2;
			if (x > INF) return false;
			//cerr << i << ' ' << ll(a) << ' ' << ll(x) << '\n';
			cnt += ll(a);
			tot += ll(x);
			if (tot > INF) return false;
		}
		{
			__int128_t a = min(N, v / i) - i;
			__int128_t x = a * (i * (i+1) + i * (i + a)) / 2;
			if (x > INF) return false;
			//cerr << i << ' ' << ll(a) << ' ' << ll(x) << '\n';
			cnt += ll(a);
			tot += ll(x);
			if (tot > INF) return false;
		}
	}
	return tot <= P;
}


int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	cin >> N >> M >> P;

	ll mi = 0;
	ll ma = ll(4e9);
	while (ma - mi > 1) {
		ll md = (ma + mi) / 2;
		if (canAfford(md)) {
			mi = md;
		} else {
			ma = md;
		}
	}
	canAfford(mi);
	ll ans = ll(min(__int128_t(N) * M, __int128_t(cnt) + (P - tot) / ma));
	cout << ans << '\n';

	return 0;
}
